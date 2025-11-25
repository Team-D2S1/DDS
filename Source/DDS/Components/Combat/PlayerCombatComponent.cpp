// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/PlayerCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "AnimInstances/Player/DDSPlayerLinkedAnimLayer.h"
#include "Character/Player/PlayerBase.h"
#include "Character/EntityBase.h"
#include "Components/Inventory/InventoryComponent.h"
#include "ETC/CustomLog.h"
#include "GameAbilitySystem/Abilities/DDSPlayerGameplayAbility.h"
#include "Items/Actor/DDSSimplePlayerWeapon.h"
#include "Items/ItemInstance/ItemInstance.h"
#include "Items/Actor/DDSCraftedPlayerWeapon.h"
#include "Items/Actor/WeaponBladePart.h"
#include "Net/UnrealNetwork.h"
#include "DDS/GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "DDSGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerController/InGamePlayerController.h"
#include "DDSTypes/DDSClassTypes.h"



ADDSCraftedPlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<ADDSCraftedPlayerWeapon>(GetCharacterCarriedWeapon(InWeaponTag));
}

ADDSCraftedPlayerWeapon* UPlayerCombatComponent::GetPlayerCurrentEquippedWeapon() const
{
	return Cast<ADDSCraftedPlayerWeapon>(GetCurrentEquippedWeapon());
}

void UPlayerCombatComponent::RegisterSpawnedWeaponById(int32 ItemId)
{
	UItemInstance* ItemInstance = Cast<APlayerBase>(GetOwningPawn())->GetInventoryComponent()->GetItemByID(ItemId);
	if (!ItemInstance)
	{
		MY_LOG(LogTemp, Error, TEXT("Item with ID %d not found."), ItemId);
		return;
	}
	if (!ItemInstance->IsValidCraftedWeapon())
	{
		MY_LOG(LogTemp, Error, TEXT("Item with ID %d is not a valid crafted weapon."), ItemId);
		return;
	}
	
}

float UPlayerCombatComponent::GetPlayerCurrentEquippedWeaponDamageAtLevel(float InLevel) const
{
	return GetPlayerCurrentEquippedWeapon()->GetBaseWeaponData().WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void UPlayerCombatComponent::Multicast_HandleWeaponUnequip_Implementation(ADDSCraftedPlayerWeapon* PreviousWeaponActor)
{
	if (!PreviousWeaponActor)
	{
		MY_LOG(LogTemp, Error, TEXT("Multicast_HandleWeaponUnequip called with nullptr PreviousWeaponActor."));
		return;
	}

	UnequipWeapon(PreviousWeaponActor);
}

void UPlayerCombatComponent::NotifyRightWeaponChanged(UItemInstance* NewWeapon)
{
	bool bIsServer = GetOwner()->HasAuthority();
	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (!PlayerBase) return;

	// 무기 교체 로직: 이전 무기 제거 → 새 무기 장착
	UItemInstance* PreviousWeapon = rightWeaponItem;
	rightWeaponItem = NewWeapon;

	// 서버에서만 실행
	if (!bIsServer)
	{
		return;
	}

	FGameplayTag WeaponTag = FGameplayTag::RequestGameplayTag(FName("Player.Weapon.Crafted"));

	// 1. 기존 무기 제거 (있는 경우)
	if (PreviousWeapon != nullptr && PreviousWeapon != NewWeapon)
	{
		// 기존 무기 해제
		ADDSCraftedPlayerWeapon* PreviousWeaponActor = GetPlayerCarriedWeaponByTag(WeaponTag);
		if (PreviousWeaponActor && GetCurrentEquippedWeapon() == PreviousWeaponActor)
		{
			Multicast_HandleWeaponUnequip(PreviousWeaponActor);
			MY_LOG(LogTemp, Log, TEXT("Unequipped previous weapon: %s"), *PreviousWeapon->GetItemName());
		}
		
		// C++로 직접 디스폰 처리
		DespawnAndUnregisterWeapon(WeaponTag);
		MY_LOG(LogTemp, Log, TEXT("Despawned previous weapon: %s"), *PreviousWeapon->GetItemName());
	}

	// 2. 새 무기 스폰
	if (NewWeapon != nullptr)
	{
		// C++로 직접 스폰 처리
		ADDSCraftedPlayerWeapon* SpawnedWeapon = SpawnAndRegisterWeapon(NewWeapon, WeaponTag);
		
		if (SpawnedWeapon)
		{
			MY_LOG(LogTemp, Log, TEXT("Spawned new weapon: %s"), *NewWeapon->GetItemName());
		}
		else
		{
			MY_LOG(LogTemp, Error, TEXT("Failed to spawn new weapon: %s"), *NewWeapon->GetItemName());
		}
	}
	else if (PreviousWeapon == nullptr)
	{
		// 둘 다 nullptr인 경우 - 아무 작업도 필요 없음
		MY_LOG(LogTemp, Log, TEXT("No weapon change - both previous and new are null"));
	}
}

void UPlayerCombatComponent::TriggerStopStaminaRegen()
{
    APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
    if (!PlayerBase) return;

    UDDSAbilitySystemComponent* ASC = PlayerBase->GetDDSAbilitySystemComponent();
    if (!ASC) return;

    if (!StopStaminaRegenEffectClass) return;

    // Use the gameplay tag as the matching key so existing effects with the tag will be removed and refreshed
    const FGameplayTag StopTag = DDSGameplayTags::Player_State_StopStaminaRegen;
    FActiveGameplayEffectHandle Handle = ASC->ApplyOrRefreshGameplayEffectToSelf(StopStaminaRegenEffectClass, 1.0f, StopTag);
    if (Handle.IsValid())
    {
        MY_LOG(LogTemp, Log, TEXT("Triggered StopStaminaRegen GE on %s."), *PlayerBase->GetName());
    }
    else
    {
        MY_LOG(LogTemp, Warning, TEXT("Failed to apply StopStaminaRegen GE on %s."), *PlayerBase->GetName());
    }
}

void UPlayerCombatComponent::TriggerDodge(const FVector& MoveInput)
{
	// 클라이언트에서 호출되면 서버로 전달
	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (!PlayerBase) return;

	UDDSAbilitySystemComponent* ASC = PlayerBase->GetDDSAbilitySystemComponent();
	if (!ASC) return;

	// 클라이언트에서도 즉시 설정 (Replication 지연 방지)
	ASC->SetLastDodgeInputDirection(MoveInput.GetSafeNormal());

	// 로컬 플레이어 또는 서버에서 직접 실행
	if (PlayerBase->HasAuthority())
	{
		Server_TriggerDodge(MoveInput);
	}
	else
	{
		// 클라이언트는 서버 RPC 호출
		Server_TriggerDodge(MoveInput);
	}
}

void UPlayerCombatComponent::Server_TriggerDodge_Implementation(FVector_NetQuantize MoveInput)
{
	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (!PlayerBase) return;

	UDDSAbilitySystemComponent* ASC = PlayerBase->GetDDSAbilitySystemComponent();
	if (!ASC) return;

	// 월드 방향을 그대로 저장 (정규화)
	ASC->SetLastDodgeInputDirection(MoveInput.GetSafeNormal());

	// Tag로 GA 트리거
	FGameplayTagContainer DodgeTags;
	DodgeTags.AddTag(DDSGameplayTags::Player_Ability_Dodge);

	ASC->TryActivateAbilitiesByTag(DodgeTags);
}


void UPlayerCombatComponent::TriggerBackstep()
{
	// 클라이언트에서 호출되면 서버로 전달
	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (!PlayerBase) return;

	UDDSAbilitySystemComponent* ASC = PlayerBase->GetDDSAbilitySystemComponent();
	if (!ASC) return;

	// 캐릭터가 현재 바라보는 반대 방향 (월드 좌표)
	FVector Backward = -PlayerBase->GetActorForwardVector();
	
	// 클라이언트에서도 즉시 설정 (Replication 지연 방지)
	ASC->SetLastDodgeInputDirection(Backward.GetSafeNormal());

	// 로컬 플레이어 또는 서버에서 직접 실행
	if (PlayerBase->HasAuthority())
	{
		Server_TriggerBackstep();
	}
	else
	{
		// 클라이언트는 서버 RPC 호출
		Server_TriggerBackstep();
	}
}

void UPlayerCombatComponent::Server_TriggerBackstep_Implementation()
{
	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (!PlayerBase) return;

	UDDSAbilitySystemComponent* ASC = PlayerBase->GetDDSAbilitySystemComponent();
	if (!ASC) return;

	// 캐릭터가 현재 바라보는 반대 방향 (월드 좌표)
	FVector Backward = -PlayerBase->GetActorForwardVector();

	ASC->SetLastDodgeInputDirection(Backward.GetSafeNormal());


	FGameplayTagContainer DodgeTags;
	DodgeTags.AddTag(DDSGameplayTags::Player_Ability_Dodge);

	ASC->TryActivateAbilitiesByTag(DodgeTags);
}

ADDSCraftedPlayerWeapon* UPlayerCombatComponent::SpawnAndRegisterWeapon(UItemInstance* WeaponItemInstance, FGameplayTag WeaponTag)
{
	if (!WeaponItemInstance)
	{
		MY_ERROR_DISPLAY(TEXT("WeaponItemInstance is nullptr"));
		return nullptr;
	}

	if (!WeaponItemInstance->IsValidCraftedWeapon())
	{
		MY_ERROR_DISPLAY(TEXT("WeaponItemInstance is not a valid crafted weapon"));
		return nullptr;
	}

	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (!PlayerBase)
	{
		MY_ERROR_DISPLAY(TEXT("PlayerBase is nullptr"));
		return nullptr;
	}

	// Grip 정보 가져오기
	UItemInstance* GripItemInstance = WeaponItemInstance->GetGripItemInstance();
	if (!GripItemInstance)
	{
		MY_ERROR_DISPLAY(TEXT("GripItemInstance is nullptr"));
		return nullptr;
	}

	// GripItemInstance의 ItemClass CDO에서 GripPart 가져오기
	TSubclassOf<UItemStaticData> GripItemClass = GripItemInstance->GetItemClass();
	if (!GripItemClass)
	{
		MY_ERROR_DISPLAY(TEXT("GripItemClass is not set"));
		return nullptr;
	}
	const UItemStaticData* GripCDO = GripItemClass->GetDefaultObject<UItemStaticData>();
	if (!GripCDO)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to get GripCDO"));
		return nullptr;
	}
	TSubclassOf<ADDSCraftedPlayerWeapon> GripClass = GripCDO->GripPart;
	if (!GripClass)
	{
		MY_ERROR_DISPLAY(TEXT("GripPart is not set in GripItemClass CDO"));
		return nullptr;
	}

	MY_LOG(LogTemp, Log, TEXT("GripClass: %s"), *GripClass->GetName());

	// Blade 정보 가져오기
	UItemInstance* BladeItemInstance = WeaponItemInstance->GetBladeItemInstance();
	if (!BladeItemInstance)
	{
		MY_ERROR_DISPLAY(TEXT("BladeItemInstance is nullptr"));
		return nullptr;
	}

	// BladeItemInstance의 ItemClass CDO에서 BladePartClass 가져오기
	TSubclassOf<UItemStaticData> BladeItemClass = BladeItemInstance->GetItemClass();
	if (!BladeItemClass)
	{
		MY_ERROR_DISPLAY(TEXT("BladeItemClass is not set"));
		return nullptr;
	}
	const UItemStaticData* BladeCDO = BladeItemClass->GetDefaultObject<UItemStaticData>();
	if (!BladeCDO)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to get BladeCDO"));
		return nullptr;
	}
	TSubclassOf<AActor> BladeClass = BladeCDO->BladePartClass;
	if (!BladeClass)
	{
		MY_ERROR_DISPLAY(TEXT("BladePartClass is not set in BladeItemClass CDO"));
		return nullptr;
	}

	MY_LOG(LogTemp, Log, TEXT("BladeClass: %s"), *BladeClass->GetName());

	// 1. Grip 무기 액터 스폰
	FActorSpawnParameters GripSpawnParams;
	GripSpawnParams.Owner = PlayerBase;
	GripSpawnParams.Instigator = PlayerBase;
	GripSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ADDSCraftedPlayerWeapon* SpawnedGrip = GetWorld()->SpawnActor<ADDSCraftedPlayerWeapon>(
		GripClass,
		PlayerBase->GetActorLocation(),
		PlayerBase->GetActorRotation(),
		GripSpawnParams
	);

	if (!SpawnedGrip)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to spawn Grip weapon actor"));
		return nullptr;
	}

	MY_LOG(LogTemp, Log, TEXT("Successfully spawned Grip weapon actor: %s"), *SpawnedGrip->GetName());

	// 2. Blade 파트 액터 스폰
	FActorSpawnParameters BladeSpawnParams;
	BladeSpawnParams.Owner = PlayerBase;
	BladeSpawnParams.Instigator = PlayerBase;
	BladeSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedBlade = GetWorld()->SpawnActor<AActor>(
		BladeClass,
		PlayerBase->GetActorLocation(),
		PlayerBase->GetActorRotation(),
		BladeSpawnParams
	);

	if (!SpawnedBlade)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to spawn Blade part actor"));
		SpawnedGrip->Destroy();
		return nullptr;
	}

	MY_LOG(LogTemp, Log, TEXT("Successfully spawned Blade actor: %s"), *SpawnedBlade->GetName());

	// 3. Blade를 Grip의 소켓에 어태치 (소켓 대 소켓)
	// Blade의 bladeTaleSocket을 Grip의 jarooHeadSocket에 붙이기
	AWeaponBladePart* BladePart = Cast<AWeaponBladePart>(SpawnedBlade);
	if (BladePart)
	{
		UStaticMeshComponent* BladeMesh = BladePart->GetBladePartMesh();
		UStaticMeshComponent* GripMesh = SpawnedGrip->GetWeaponMesh();
		
		if (BladeMesh && GripMesh)
		{
			FName GripSocketName = TEXT("jarooHeadSocket"); // Grip의 소켓
			
			// Blade 액터 전체를 Grip의 jarooHeadSocket에 어태치
			SpawnedBlade->AttachToComponent(
				GripMesh,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				GripSocketName
			);
			
			MY_LOG(LogTemp, Log, TEXT("Successfully attached Blade to Grip socket: %s"), *GripSocketName.ToString());
		}
		else
		{
			MY_LOG(LogTemp, Error, TEXT("BladeMesh or GripMesh is nullptr"));
		}
		
		SpawnedGrip->SetBladeActor(BladePart);
		MY_LOG(LogTemp, Log, TEXT("Set BladePart to Grip weapon"));
	}
	else
	{
		MY_LOG(LogTemp, Warning, TEXT("SpawnedBlade is not a WeaponBladePart, trying generic attach"));
		
		// WeaponBladePart가 아닌 경우 일반 Actor로 어태치
		UStaticMeshComponent* GripMesh = SpawnedGrip->GetWeaponMesh();
		if (GripMesh)
		{
			FName GripSocketName = TEXT("jarooHeadSocket");
			SpawnedBlade->AttachToComponent(
				GripMesh,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				GripSocketName
			);
			MY_LOG(LogTemp, Log, TEXT("Attached Blade to Grip socket: %s"), *GripSocketName.ToString());
		}
	}

	// 4. 무기 초기화
	SpawnedGrip->SetParentItemId(WeaponItemInstance->GetItemId());
	SpawnedGrip->SetWeaponItemInstance(WeaponItemInstance);

	// 5. 무기 등록 (CharacterCarriedWeaponArray에 추가)
	RegisterSpawnedWeapon(WeaponTag, SpawnedGrip, false);

	// 6. Grip을 플레이어 메시의 SwordSideSocket에 어태치 (장착 전 대기 위치)
	// 무기 등록 후에 어태치해야 모든 클라이언트에서 올바르게 복제됨
	USkeletalMeshComponent* PlayerMesh = PlayerBase->GetMesh();
	if (PlayerMesh)
	{
		FName SocketName = TEXT("SwordSideSocket");
		SpawnedGrip->AttachToComponent(PlayerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		MY_LOG(LogTemp, Log, TEXT("Attached weapon to SwordSideSocket"));
	}

	MY_LOG(LogTemp, Log, TEXT("Spawned and registered crafted weapon: %s with tag: %s"), 
		*WeaponItemInstance->GetItemName(), *WeaponTag.ToString());

	return SpawnedGrip;
}

void UPlayerCombatComponent::DespawnAndUnregisterWeapon(FGameplayTag WeaponTag)
{
	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (!PlayerBase)
	{
		MY_ERROR_DISPLAY(TEXT("PlayerBase is nullptr"));
		return;
	}

	// 1. 무기 찾기
	ADDSCraftedPlayerWeapon* Weapon = GetPlayerCarriedWeaponByTag(WeaponTag);
	if (!Weapon)
	{
		MY_LOG(LogTemp, Warning, TEXT("Weapon with tag %s not found for despawn"), *WeaponTag.ToString());
		return;
	}

	// 2. HandleUnequipWeapon 로직: 무기 데이터 캐싱
	FDDSBaseWeaponData CachedWeaponData = Weapon->GetBaseWeaponData();

	// 3. 애니메이션 레이어 언링크 (서버에서만, 모든 클라이언트에 멀티캐스트)
	if (PlayerBase->HasAuthority())
	{
		if (CachedWeaponData.WeaponAnimLayerToLink)
		{
			PlayerBase->Multicast_UnlinkAnimLayer(CachedWeaponData.WeaponAnimLayerToLink);
			MY_LOG(LogTemp, Log, TEXT("Unlinked anim layer: %s"), *CachedWeaponData.WeaponAnimLayerToLink->GetName());
		}
	}

	// 4. 입력 매핑 컨텍스트 제거 (로컬 컨트롤러에서만)
	if (PlayerBase->IsLocallyControlled())
	{
		AInGamePlayerController* PC = Cast<AInGamePlayerController>(PlayerBase->GetController());
		if (PC)
		{
			UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
			if (InputSubsystem && CachedWeaponData.WeaponInputMappingContext)
			{
				FModifyContextOptions Options;
				Options.bIgnoreAllPressedKeysUntilRelease = true;
				InputSubsystem->RemoveMappingContext(CachedWeaponData.WeaponInputMappingContext, Options);
				MY_LOG(LogTemp, Log, TEXT("Removed input mapping context"));
			}
		}
	}

	// 5. 무기 능력 제거 (서버에서만)
	if (PlayerBase->HasAuthority())
	{
		UDDSAbilitySystemComponent* ASC = PlayerBase->GetDDSAbilitySystemComponent();
		if (ASC)
		{
			TArray<FGameplayAbilitySpecHandle> GrantedHandles = Weapon->GetGrantedAbilitySpecHandles();
			ASC->RemoveGrantedPlayerWeaponAbilities(GrantedHandles);
			MY_LOG(LogTemp, Log, TEXT("Removed %d weapon abilities"), GrantedHandles.Num());
		}
	}

	// 6. 무기 등록 해제
	int32 ItemId = Weapon->GetItemId();
	UnregisterSpawnedWeaponById(ItemId);

	// 7. 액터 파괴
	Weapon->Destroy();

	// 8. CurrentEquippedWeaponTag 초기화
	CurrentEquippedWeaponTag = FGameplayTag::EmptyTag;

	MY_LOG(LogTemp, Log, TEXT("Despawned and unregistered weapon with tag: %s"), *WeaponTag.ToString());
}

void UPlayerCombatComponent::Multicast_AttachWeaponToSocket_Implementation(ADDSCraftedPlayerWeapon* Weapon, FName SocketName)
{
	if (!Weapon)
	{
		MY_LOG(LogTemp, Error, TEXT("Weapon is nullptr in Multicast_AttachWeaponToSocket"));
		return;
	}

	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (!PlayerBase)
	{
		MY_LOG(LogTemp, Error, TEXT("PlayerBase is nullptr"));
		return;
	}

	USkeletalMeshComponent* PlayerMesh = PlayerBase->GetMesh();
	if (!PlayerMesh)
	{
		MY_LOG(LogTemp, Error, TEXT("PlayerMesh is nullptr"));
		return;
	}

	// 소켓에 부착
	Weapon->AttachToComponent(
		PlayerMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		SocketName
	);

	bool bIsServer = GetOwner()->HasAuthority();
	MY_LOG(LogTemp, Log, TEXT("[%s] Attached weapon %s to socket %s"), 
		bIsServer ? TEXT("Server") : TEXT("Client"),
		*Weapon->GetName(), 
		*SocketName.ToString());
}

ADDSCraftedPlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTagSafe(FGameplayTag InWeaponTag, bool bRequestFromServer)
{
	// 먼저 로컬에서 무기 찾기
	ADDSCraftedPlayerWeapon* Weapon = GetPlayerCarriedWeaponByTag(InWeaponTag);
	
	if (Weapon)
	{
		// 무기를 찾았으면 바로 반환
		return Weapon;
	}

	// 클라이언트이고 무기를 찾지 못했으며 서버 요청이 활성화된 경우
	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (PlayerBase && !PlayerBase->HasAuthority() && bRequestFromServer)
	{
		MY_LOG(LogTemp, Warning, TEXT("[Client] Weapon with tag %s not found locally, requesting from server"), *InWeaponTag.ToString());
		Server_RequestWeaponByTag(InWeaponTag);
	}
	
	return nullptr;
}


void UPlayerCombatComponent::UnequipWeapon(ADDSCraftedPlayerWeapon* WeaponToUnequip)
{
	if (!WeaponToUnequip)
	{
		MY_LOG(LogTemp, Error, TEXT("WeaponToUnequip is nullptr"));
		return;
	}

	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (!PlayerBase)
	{
		MY_LOG(LogTemp, Error, TEXT("PlayerBase is nullptr"));
		return;
	}

	// 1. 무기 데이터 가져오기
	FDDSBaseWeaponData WeaponData = WeaponToUnequip->GetBaseWeaponData();

	MY_LOG(LogTemp, Warning, TEXT("[UnequipWeapon] Starting unequip for weapon: %s"), *WeaponToUnequip->GetName());

	// 2. 애니메이션 레이어 언링크 (서버에서만)
	if (PlayerBase->HasAuthority())
	{
		if (WeaponData.WeaponAnimLayerToLink)
		{
			PlayerBase->Multicast_UnlinkAnimLayer(WeaponData.WeaponAnimLayerToLink);
			MY_LOG(LogTemp, Log, TEXT("Unlinked anim layer: %s"), *WeaponData.WeaponAnimLayerToLink->GetName());
		}
	}

	// 3. 입력 매핑 컨텍스트 제거 (로컬 컨트롤러에서만)
	if (PlayerBase->IsLocallyControlled())
	{
		AInGamePlayerController* PC = Cast<AInGamePlayerController>(PlayerBase->GetController());
		if (PC)
		{
			UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
			if (InputSubsystem && WeaponData.WeaponInputMappingContext)
			{
				FModifyContextOptions Options;
				Options.bIgnoreAllPressedKeysUntilRelease = true;
				InputSubsystem->RemoveMappingContext(WeaponData.WeaponInputMappingContext, Options);
				MY_LOG(LogTemp, Log, TEXT("Removed input mapping context"));
			}
		}
	}

	// 4. 무기 능력 제거 (서버에서만)
	if (PlayerBase->HasAuthority())
	{
		UDDSAbilitySystemComponent* ASC = PlayerBase->GetDDSAbilitySystemComponent();
		if (ASC)
		{
			TArray<FGameplayAbilitySpecHandle> GrantedHandles = WeaponToUnequip->GetGrantedAbilitySpecHandles();
			ASC->RemoveGrantedPlayerWeaponAbilities(GrantedHandles);
			MY_LOG(LogTemp, Log, TEXT("Removed %d weapon abilities"), GrantedHandles.Num());
		}
	}

	// 5. CurrentEquippedWeaponTag 초기화
	CurrentEquippedWeaponTag = FGameplayTag::EmptyTag;

	MY_LOG(LogTemp, Warning, TEXT("[UnequipWeapon] Completed unequip for weapon: %s"), *WeaponToUnequip->GetName());
}

void UPlayerCombatComponent::Server_NotifyWeaponEquipped_Implementation(ADDSCraftedPlayerWeapon* EquippedWeapon)
{
	if (!EquippedWeapon)
	{
		return;
	}

	const FDDSBaseWeaponData& WeaponData = EquippedWeapon->GetBaseWeaponData();
	if (WeaponData.WeaponAnimLayerToLink)
	{
		UE_LOG(LogTemp, Log, TEXT("[Server_NotifyWeaponEquipped] Broadcasting weapon equip to all clients with AnimLayer: %s"), 
			*WeaponData.WeaponAnimLayerToLink->GetName());
		
		Multicast_NotifyWeaponEquipped(EquippedWeapon, WeaponData.WeaponAnimLayerToLink);
	}
}

void UPlayerCombatComponent::Multicast_NotifyWeaponEquipped_Implementation(ADDSCraftedPlayerWeapon* EquippedWeapon, TSubclassOf<UAnimInstance> AnimLayerClass)
{
	if (!EquippedWeapon || !AnimLayerClass)
	{
		return;
	}

	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (!PlayerBase)
	{
		return;
	}

	const bool bIsServer = PlayerBase->HasAuthority();
	
	UE_LOG(LogTemp, Warning, TEXT("[Multicast_NotifyWeaponEquipped] %s - Role: %d, RemoteRole: %d, AnimLayer: %s"),
		bIsServer ? TEXT("Server") : TEXT("Client"),
		static_cast<int32>(PlayerBase->GetLocalRole()),
		static_cast<int32>(PlayerBase->GetRemoteRole()),
		*AnimLayerClass->GetName());

	// 애니메이션 레이어 링크 (모든 클라이언트에서)
	if (USkeletalMeshComponent* Mesh = PlayerBase->GetMesh())
	{
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			UE_LOG(LogTemp, Log, TEXT("[Multicast_NotifyWeaponEquipped] Current AnimInstance: %s"), *AnimInstance->GetName());
			
			// 직접 LinkAnimClassLayers 호출
			Mesh->LinkAnimClassLayers(AnimLayerClass);
			UE_LOG(LogTemp, Warning, TEXT("[Multicast_NotifyWeaponEquipped] %s - Linked anim layer: %s"),
				bIsServer ? TEXT("Server") : TEXT("Client"),
				*AnimLayerClass->GetName());
		}
	}
}

// void UPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
// 	FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// 	if (FocusedObject)
// 	{
// 		APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
// 		if (!PlayerBase) return;
// 		FVector TargetLocation = FocusedObject->GetActorLocation();
// 		// 플레이어가 적을 바라보도록
// 		FRotator PlayerLookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerBase->GetActorLocation(), TargetLocation);
// 		PlayerBase->SetActorRotation(FRotator(0.f, PlayerLookAtRotation.Yaw, 0.f));
// 		
// 		// 스프링암 각도 조정, 대상과 반대편이고 살짝 위로
// 		USpringArmComponent* SpringArm = PlayerBase->GetSpringArmComponent();
// 		if (!SpringArm) return;
// 		FVector PlayerLocation = PlayerBase->GetActorLocation();
// 		FRotator LookAtSpringArmRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);
// 		// 높게 
// 		LookAtSpringArmRotation.Pitch = -30.f;
// 		SpringArm->SetWorldRotation(LookAtSpringArmRotation);
// 		// 카메라 방향고정
// 		UCameraComponent* Cam = PlayerBase->GetCameraComponent();
// 		if (!Cam) return;
// 		FVector CameraLocation = Cam->GetComponentLocation();
// 		
// 		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TargetLocation);
// 		Cam->SetWorldRotation(LookAtRotation);
// 	}
// }

