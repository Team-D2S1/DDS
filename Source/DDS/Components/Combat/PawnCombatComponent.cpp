// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DDSGameplayTags.h"
#include "Character/Player/PlayerBase.h"
#include "Components/BoxComponent.h"
#include "Components/Inventory/InventoryComponent.h"
#include "ETC/CustomLog.h"
#include "ETC/DDSFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Items/Actor/DDSWeaponBase.h"

void UPawnCombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPawnCombatComponent, CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTag, ADDSWeaponBase* InWeapon,
                                                 bool bRegisterAsEquippedWeapon)
{
	bool bIsServer = GetOwningPawn()->HasAuthority();
	MY_LOG(LogTemp, Log, TEXT("[%s] RegisterSpawnedWeapon called with WeaponTag: %s"), 
	       bIsServer ? TEXT("Server") : TEXT("Client"), *InWeaponTag.ToString());
	if (CharacterCarriedWeaponMap.Contains(InWeaponTag))
	{
		MY_LOG(LogTemp, Error, TEXT("Weapon Tag %s is already registered."), *InWeaponTag.ToString());
		return;
	}
  
	CharacterCarriedWeaponMap.Emplace(InWeaponTag, InWeapon);

	// UFunction() 이면 DynamicDelegate로 해야함
	InWeapon->OnWeaponHitTarget.BindUObject(this, &UPawnCombatComponent::OnHitTarget);
	InWeapon->OnWeaponPulledFromTarget.BindUObject(this, &UPawnCombatComponent::OnPulledFromTarget);
	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTag;
	}

	bool hasAuthority = GetOwningPawn()->HasAuthority();
	if (hasAuthority)
	{
		//클라이언트도 RegisterSpawnedWeapon을 호출하게 하기 위해
		InWeapon->SetOwnerPawn(GetOwningPawn());
		InWeapon->SetWeaponTag(InWeaponTag);
	}
	MY_LOG_DISPLAY_NET(hasAuthority, TEXT("A Weapon %s (Tag: %s) is registered."), *InWeapon->GetName(), *InWeaponTag.ToString());
}



void UPawnCombatComponent::UnregisterSpawnedWeaponById(int32 ItemId)
{
	for (auto& Pair : CharacterCarriedWeaponMap)
	{
		if (Pair.Value->GetItemId() == ItemId)
		{
			Pair.Value->OnWeaponHitTarget.Unbind();
			Pair.Value->OnWeaponPulledFromTarget.Unbind();
			CharacterCarriedWeaponMap.Remove(Pair.Key);
			MY_LOG(LogTemp, Log, TEXT("A Weapon %s is unregistered."), *Pair.Value->GetName());
			return;
		}
	}
	MY_LOG(LogTemp, Error, TEXT("Weapon with ItemId %d is not registered."), ItemId);
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
				// MY_CLOG_DISPLAY_NET(FColor::Green, GetOwningPawn()->HasAuthority(), TEXT("Current Weapon is enabled."));
			}
			else
			{
				CollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
				OverlappedActors.Empty();
				// MY_CLOG_DISPLAY_NET(FColor::Red, GetOwningPawn()->HasAuthority(), TEXT("Current Weapon is disabled."));
			}
			
		}else
		{
			MY_CLOG_DISPLAY_NET(FColor::Red, GetOwningPawn()->HasAuthority(), TEXT("Current Weapon is not equipped."));
		}
	}
	//TODO : 무기가 없는 경우 처리
}

bool UPawnCombatComponent::IsParrying() const
{
	return UDDSFunctionLibrary::NativeDoesActorHasTag(GetOwningPawn(), DDSGameplayTags::Shared_State_Parrying);
}

void UPawnCombatComponent::OnRep_CurrentEquippedWeaponTag()
{
	if (CurrentEquippedWeaponTag.IsValid())
		MY_LOG(LogTemp, Log, TEXT("CurrentEquippedWeaponTag is changed(Rep) to %s"), *CurrentEquippedWeaponTag.ToString());
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
	OverlappedActors.Add(InTargetActor); // 어차피 위에서 확인함

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
	// TODO : 무기 정보도 넘기기

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
