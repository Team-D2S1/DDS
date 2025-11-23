// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DDSGameplayTags.h"
#include "Components/BoxComponent.h"
#include "Components/Inventory/InventoryComponent.h"
#include "ETC/CustomLog.h"
#include "ETC/DDSFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Items/Actor/DDSWeaponBase.h"
#include "Character/EntityBase.h"
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AnimInstances/Player/DDSPlayerLinkedAnimLayer.h"

void UPawnCombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPawnCombatComponent, CurrentEquippedWeaponTag);
	// 배열을 즉시 복제하도록 설정 - COND_None 사용
	DOREPLIFETIME_CONDITION(UPawnCombatComponent, CharacterCarriedWeaponArray, COND_None);
}

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTag, ADDSWeaponBase* InWeapon,
                                                 bool bRegisterAsEquippedWeapon)
{
	if (!InWeapon)
	{
		MY_LOG(LogTemp, Error, TEXT("RegisterSpawnedWeapon called with nullptr weapon."));
		return;
	}


	// 서버에서만 호출 가능
	bool bIsServer = GetOwningPawn()->HasAuthority();
	MY_LOG(LogTemp, Log, TEXT("[%s] RegisterSpawnedWeapon called with WeaponTag: %s"),
	       bIsServer ? TEXT("Server") : TEXT("Client"), *InWeaponTag.ToString());
	
	if (bIsServer)
	{
		// 서버에서는 Multicast로 모든 클라이언트(서버 포함)에 알림
		Multicast_RegisterSpawnedWeapon(InWeaponTag, InWeapon, bRegisterAsEquippedWeapon);
	}
	else
	{
		MY_LOG(LogTemp, Error, TEXT("RegisterSpawnedWeapon can only be called on the server."));
	}
}


void UPawnCombatComponent::Multicast_RegisterSpawnedWeapon_Implementation(FGameplayTag InWeaponTag, ADDSWeaponBase* InWeapon,
	bool bRegisterAsEquippedWeapon)
{
	bool bIsServer = GetOwningPawn()->HasAuthority();

	// 중복 체크
	for (const FWeaponMapEntry& Entry : CharacterCarriedWeaponArray)
	{
		if (Entry.WeaponTag.MatchesTagExact(InWeaponTag))
		{
			MY_LOG(LogTemp, Warning, TEXT("[%s] Weapon Tag %s is already registered."),
			       bIsServer ? TEXT("Server") : TEXT("Client"), *InWeaponTag.ToString());
			return;
		}
	}

	FWeaponMapEntry NewEntry;
	NewEntry.WeaponTag = InWeaponTag;
	NewEntry.Weapon = InWeapon;
	CharacterCarriedWeaponArray.Add(NewEntry);
	
	if (InWeapon)
	{
		InWeapon->OnWeaponHitTarget.BindUObject(this, &UPawnCombatComponent::OnHitTarget);
		InWeapon->OnWeaponPulledFromTarget.BindUObject(this, &UPawnCombatComponent::OnPulledFromTarget);
	}
	
	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTag;
	}

	MY_LOG(LogTemp, Log, TEXT("[%s] Weapon %s (Tag: %s) is registered via Multicast."),
	       bIsServer ? TEXT("Server") : TEXT("Client"),
	       InWeapon ? *InWeapon->GetName() : TEXT("nullptr"), 
	       *InWeaponTag.ToString());
}

ADDSWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeapon(FGameplayTag InWeaponTag) const
{
	for (const FWeaponMapEntry& Entry : CharacterCarriedWeaponArray)
	{
		if (Entry.WeaponTag.MatchesTagExact(InWeaponTag))
		{
			return Entry.Weapon;
		}
	}
	return nullptr;
}

ADDSWeaponBase* UPawnCombatComponent::GetCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}
	return GetCharacterCarriedWeapon(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::UnregisterSpawnedWeaponById(int32 ItemId)
{
	if (GetOwningPawn()->HasAuthority())
	{
		MY_LOG(LogTemp, Log, TEXT("UnregisterSpawnedWeaponById called with ItemId: %d"), ItemId);
		Multicast_UnregisterSpawnedWeaponById(ItemId);
	}
	else
	{
		MY_LOG(LogTemp, Error, TEXT("UnregisterSpawnedWeaponById can only be called on the server."));
	}
}

void UPawnCombatComponent::Multicast_UnregisterSpawnedWeaponById_Implementation(int32 ItemId)
{
	bool bIsServer = GetOwningPawn()->HasAuthority();
	for (int32 i = CharacterCarriedWeaponArray.Num() - 1; i >= 0; --i)
	{
		if (CharacterCarriedWeaponArray[i].Weapon && CharacterCarriedWeaponArray[i].Weapon->GetItemId() == ItemId)
		{
			CharacterCarriedWeaponArray[i].Weapon->OnWeaponHitTarget.Unbind();
			CharacterCarriedWeaponArray[i].Weapon->OnWeaponPulledFromTarget.Unbind();
			MY_LOG(LogTemp, Log, TEXT("[%s] Weapon with ItemId %d (Tag: %s) is unregistered."),
			       bIsServer ? TEXT("Server") : TEXT("Client"), ItemId, *CharacterCarriedWeaponArray[i].WeaponTag.ToString());
			CharacterCarriedWeaponArray.RemoveAt(i);
			return;
		}
	}
	MY_LOG(LogTemp, Error, TEXT("[%s] Weapon with ItemId %d not found for unregistration."),
	       bIsServer ? TEXT("Server") : TEXT("Client"), ItemId);
}

void UPawnCombatComponent::HandleWeaponEquip(ADDSWeaponBase* WeaponToEquip)
{
	if (!WeaponToEquip)
	{
		MY_LOG(LogTemp, Warning, TEXT("WeaponToEquip is null"));
		return;
	}

	AEntityBase* OwnerEntity = Cast<AEntityBase>(GetOwner());
	if (!OwnerEntity)
	{
		MY_LOG(LogTemp, Warning, TEXT("Owner is not AEntityBase"));
		return;
	}

	// 무기 데이터 가져오기
	FDDSBaseWeaponData WeaponData = WeaponToEquip->GetBaseWeaponData();
	const bool bIsServer = GetOwnerRole() == ROLE_Authority;

	// 1. 애니메이션 레이어 링크 (서버에서만 Multicast 호출)
	if (bIsServer && WeaponData.WeaponAnimLayerToLink)
	{
		MY_LOG(LogTemp, Log, TEXT("[Server] Calling Multicast_LinkAnimLayer for weapon: %s"), *WeaponToEquip->GetName());
		OwnerEntity->Multicast_LinkAnimLayer(WeaponData.WeaponAnimLayerToLink);
	}

	// 2. 능력 부여 (서버에서만)
	if (bIsServer)
	{
		UDDSAbilitySystemComponent* ASC = OwnerEntity->GetDDSAbilitySystemComponent();
		if (ASC && WeaponData.DefaultWeaponAbilities.Num() > 0)
		{
			TArray<FGameplayAbilitySpecHandle> GrantedHandles;
			ASC->GrantPlayerWeaponAbilities(WeaponData.DefaultWeaponAbilities, 1, GrantedHandles);

			// 무기에 핸들 저장
			WeaponToEquip->AssignGrantedAbilitySpecHandles(GrantedHandles);

			MY_LOG(LogTemp, Log, TEXT("[Server] Granted %d abilities to weapon: %s"),
				GrantedHandles.Num(), *WeaponToEquip->GetName());
		}
	}

	// 3. IMC 추가 (로컬 플레이어만)
	if (OwnerEntity->IsLocallyControlled() && WeaponData.WeaponInputMappingContext)
	{
		APlayerController* PC = Cast<APlayerController>(OwnerEntity->GetController());
		if (PC)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(WeaponData.WeaponInputMappingContext, 1);
				MY_LOG(LogTemp, Log, TEXT("[Client] Added IMC for weapon: %s"), *WeaponToEquip->GetName());
			}
		}
	}

}

void UPawnCombatComponent::HandleWeaponUnequip(ADDSWeaponBase* WeaponToUnequip)
{
	if (!WeaponToUnequip)
	{
		MY_LOG(LogTemp, Warning, TEXT("WeaponToUnequip is null"));
		return;
	}

	AEntityBase* OwnerEntity = Cast<AEntityBase>(GetOwner());
	if (!OwnerEntity)
	{
		MY_LOG(LogTemp, Warning, TEXT("Owner is not AEntityBase"));
		return;
	}

	// 무기 데이터 가져오기
	FDDSBaseWeaponData WeaponData = WeaponToUnequip->GetBaseWeaponData();
	const bool bIsServer = GetOwnerRole() == ROLE_Authority;

	// 1. 애니메이션 레이어 언링크 (서버에서만 Multicast 호출)
	if (bIsServer && WeaponData.WeaponAnimLayerToLink)
	{
		MY_LOG(LogTemp, Log, TEXT("[Server] Calling Multicast_UnlinkAnimLayer for weapon: %s"), *WeaponToUnequip->GetName());
		OwnerEntity->Multicast_UnlinkAnimLayer(WeaponData.WeaponAnimLayerToLink);
	}

	// 2. 능력 제거 (서버에서만)
	if (bIsServer)
	{
		UDDSAbilitySystemComponent* ASC = OwnerEntity->GetDDSAbilitySystemComponent();
		if (ASC)
		{
			TArray<FGameplayAbilitySpecHandle> GrantedHandles = WeaponToUnequip->GetGrantedAbilitySpecHandles();
			ASC->RemoveGrantedPlayerWeaponAbilities(GrantedHandles);
			
			MY_LOG(LogTemp, Log, TEXT("[Server] Removed %d abilities from weapon: %s"), 
				GrantedHandles.Num(), *WeaponToUnequip->GetName());
		}
	}

	// 3. IMC 제거 (로컬 플레이어만)
	if (OwnerEntity->IsLocallyControlled() && WeaponData.WeaponInputMappingContext)
	{
		APlayerController* PC = Cast<APlayerController>(OwnerEntity->GetController());
		if (PC)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(WeaponData.WeaponInputMappingContext);
				MY_LOG(LogTemp, Log, TEXT("[Client] Removed IMC for weapon: %s"), *WeaponToUnequip->GetName());
			}
		}
	}
}


void UPawnCombatComponent::ToggleWeaponCollision(bool bEnable, EToggleCollisionType InDamageType)
{
	if (InDamageType == EToggleCollisionType::CurrentEquippedWeapon)
	{
		ADDSWeaponBase* CurrentWeapon = GetCurrentEquippedWeapon();
		if (CurrentWeapon && IsValid(CurrentWeapon))
		{
			UBoxComponent* CollisionBox = CurrentWeapon->GetWeaponCollsionBox();
			if (!CollisionBox)
			{
				MY_LOG(LogTemp, Error, TEXT("Current Weapon's CollisionBox is nullptr."));
				return;
			}
			if (bEnable)
			{
				CollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
			}
			else
			{
				CollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
				OverlappedActors.Empty();
			}
			
		}else
		{
			MY_CLOG_DISPLAY_NET(FColor::Red, GetOwningPawn()->HasAuthority(), TEXT("Current Weapon is not equipped."));
		}
	}
}

bool UPawnCombatComponent::IsParrying() const
{
	return UDDSFunctionLibrary::NativeDoesActorHasTag(GetOwningPawn(), DDSGameplayTags::Shared_State_Parrying);
}

void UPawnCombatComponent::Server_RequestWeaponByTag_Implementation(FGameplayTag InWeaponTag)
{
	MY_LOG(LogTemp, Log, TEXT("[Server] Received weapon request for tag: %s"), *InWeaponTag.ToString());
	
	// 서버에서 무기를 찾아서 다시 멀티캐스트로 동기화
	ADDSWeaponBase* FoundWeapon = GetCharacterCarriedWeapon(InWeaponTag);
	if (FoundWeapon)
	{
		MY_LOG(LogTemp, Log, TEXT("[Server] Found weapon, re-broadcasting to clients: %s"), *FoundWeapon->GetName());
		Multicast_RegisterSpawnedWeapon(InWeaponTag, FoundWeapon, false);
	}
	else
	{
		MY_LOG(LogTemp, Warning, TEXT("[Server] Weapon with tag %s not found on server"), *InWeaponTag.ToString());
	}
}

void UPawnCombatComponent::OnRep_CurrentEquippedWeaponTag()
{
	if (CurrentEquippedWeaponTag.IsValid())
		MY_LOG(LogTemp, Log, TEXT("CurrentEquippedWeaponTag is changed(Rep) to %s"), *CurrentEquippedWeaponTag.ToString());
}

void UPawnCombatComponent::OnRep_CharacterCarriedWeaponArray()
{
	// Multicast RPC로 처리하므로 여기서는 로그만 출력
	MY_LOG(LogTemp, Log, TEXT("OnRep_CharacterCarriedWeaponArray called - Array size: %d"), CharacterCarriedWeaponArray.Num());
	for (const auto& Entry : CharacterCarriedWeaponArray)
	{
		if (Entry.Weapon)
		{
			MY_LOG(LogTemp, Log, TEXT("  - Tag: %s, Weapon: %s"),
				*Entry.WeaponTag.ToString(), *Entry.Weapon->GetName());
		}
	}
}


void UPawnCombatComponent::OnHitTarget(AActor* InTargetActor)
{
	if (OverlappedActors.Contains(InTargetActor))
	{
		return;
	}
	bool bIsServer = GetOwningPawn()->HasAuthority();
	MY_LOG(LogTemp, Log, TEXT("[%s] OnHitTarget called with Target: %s"), 
	       bIsServer ? TEXT("Server") : TEXT("Client"), *InTargetActor->GetName());
	OverlappedActors.Add(InTargetActor);

	const bool bIsTargetParrying = UDDSFunctionLibrary::NativeDoesActorHasTag(InTargetActor, DDSGameplayTags::Shared_State_Parrying);
	const bool bIsAttackerUnparryable = UDDSFunctionLibrary::NativeDoesActorHasTag(GetOwningPawn(), DDSGameplayTags::Shared_State_CanNotBeParried);
	bool bIsValidHit = true;
	if (bIsTargetParrying && !bIsAttackerUnparryable)
	{
		bIsValidHit = false;
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = InTargetActor;

	if (bIsValidHit)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		DDSGameplayTags::Shared_Event_MeleeHit_Start,
		EventData);
		MY_LOG(LogTemp, Log, TEXT("Hit Target %s"), *InTargetActor->GetName());
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			InTargetActor,
			DDSGameplayTags::Shared_Event_Parry_Success,
			EventData);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		 	GetOwningPawn(),
			DDSGameplayTags::Shared_Event_AttackParried,
			EventData);
		MY_LOG(LogTemp, Log, TEXT("Target %s parried the attack!"), *InTargetActor->GetName());
	}
	
}

void UPawnCombatComponent::OnPulledFromTarget(AActor* InTargetActor)
{
	if (!OverlappedActors.Contains(InTargetActor))
	{
		return;
	}
	OverlappedActors.Remove(InTargetActor);
	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = InTargetActor;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		DDSGameplayTags::Shared_Event_MeleeHit_End,
		EventData);
}

