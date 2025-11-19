// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/PlayerCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "AnimInstances/Player/DDSPlayerLinkedAnimLayer.h"
#include "Character/Player/PlayerBase.h"
#include "Components/Inventory/InventoryComponent.h"
#include "ETC/CustomLog.h"
#include "GameAbilitySystem/Abilities/DDSPlayerGameplayAbility.h"
#include "Items/Actor/DDSSimplePlayerWeapon.h"
#include "Items/ItemInstance/ItemInstance.h"
#include "Items/Actor/DDSCraftedPlayerWeapon.h"
#include "Net/UnrealNetwork.h"
#include "DDS/GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "DDSGameplayTags.h"



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

void UPlayerCombatComponent::NotifyRightWeaponChanged(UItemInstance* NewWeapon)
{
	bool bIsServer = GetOwner()->HasAuthority();

	// 무기 제거 혹은 등륵과정 진행
	// 따로 함수 만드는게 낫지만 c++은 함수 새로 만들기 번거로우므로 일단 이렇게
	
	
	if (NewWeapon == nullptr)
	{
		if (rightWeaponItem == nullptr)
		{
			MY_LOG(LogTemp, Log, TEXT("NewWeapon is null in NotifyRightWeaponChanged but rightWeaponItem is also null"));
			return; // 이미 무기가 없는 상태
		}
		// 무기 제거 과정
		MY_LOG(LogTemp, Log, TEXT("NewWeapon is null in NotifyRightWeaponChanged"));
		APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
		if (!PlayerBase) return;
		UAbilitySystemComponent* ASC = PlayerBase->GetAbilitySystemComponent();
		if (!ASC) return;

		// 해당능력에서 해야하는것들
		// 1. (장착중이었던 경우) 무기 능력 제거
		// 2. 무기 액터 제거
		if (bIsServer)
		{
			FGameplayAbilitySpec spec(DespawnCraftedWeaponAbilityClass);
			spec.SourceObject = rightWeaponItem; // 무기 아이템을 소스로 설정
			spec.Level  = 1;
			ASC->GiveAbility(spec);
			rightWeaponItem = nullptr;
			}
		return;
	}
	if (rightWeaponItem == NewWeapon)
	{
		MY_LOG(LogTemp, Log, TEXT("NewWeapon is the same as rightWeaponItem in NotifyRightWeaponChanged"));
		return; // 이미 같은 무기가 장착된 상태
	}

	if (rightWeaponItem != nullptr)
	{
		// 기존 무기 제거 과정
		MY_LOG(LogTemp, Log, TEXT("Replacing existing weapon in NotifyRightWeaponChanged"));
		APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
		if (!PlayerBase) return;
		UAbilitySystemComponent* ASC = PlayerBase->GetAbilitySystemComponent();
		if (!ASC) return;

		// 해당능력에서 해야하는것들
		// 1. (장착중이었던 경우) 무기 능력 제거
		// 2. 무기 액터 제거
		if (bIsServer)
		{
			FGameplayAbilitySpec spec(DespawnCraftedWeaponAbilityClass);
			spec.SourceObject = rightWeaponItem; // 무기 아이템을 소스로 설정
			spec.Level  = 1;
			ASC->GiveAbility(spec);
		}
	}
	
	rightWeaponItem = NewWeapon; // 현재 장착된 무기 아이템을 업데이트(생성할때는 미리 설정)
	if (!bIsServer)
	{
		return;
	}
	if (SpawnCraftedWeaponAbilityClass)
	{
		APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
		if (!PlayerBase) return;
		UAbilitySystemComponent* ASC = PlayerBase->GetAbilitySystemComponent();
		if (!ASC) return;

		FGameplayAbilitySpec spec(SpawnCraftedWeaponAbilityClass);
		spec.SourceObject = ASC->GetAvatarActor();
		spec.Level  = 1;
		ASC->GiveAbility(spec);
		// 플레이어가 소지한 무기 아이템을 능력으로 등록
		MY_LOG(LogTemp, Log, TEXT("Created crafted weapon ability for %s"), *NewWeapon->GetItemName());
	}
	else
	{
		MY_LOG(LogTemp, Error, TEXT("CreateCraftedWeaponAbilityClass is not set"));
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

void UPlayerCombatComponent::TriggerDodge(const FVector2D& MoveInput)
{
	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (!PlayerBase) return;

	UDDSAbilitySystemComponent* ASC = PlayerBase->GetDDSAbilitySystemComponent();
	if (!ASC) return;

	// 2D 입력을 월드 기준 방향 벡터로 변환 (컨트롤러 Yaw 기준)
	const FRotator ControlRot(0.f, PlayerBase->GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X);
	const FVector Right   = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y);

	FVector WorldDir = Forward * MoveInput.Y + Right * MoveInput.X;
	WorldDir = WorldDir.GetSafeNormal();

	if (!WorldDir.IsNearlyZero())
	{
		ASC->SetLastDodgeInputDirection(WorldDir);
	}

	MY_LOG(LogTemp, Log, TEXT("TriggerDodge: MoveInput=%s, WorldDir=%s"), *MoveInput.ToString(), *WorldDir.ToString());

	// 실제 구르기 GA는 블루프린트로 구현: 여기서는 방향 캐시만 하고 GA 활성화는
	// 입력 태그 기반 또는 블루프린트에서 처리하도록 남겨둔다.
}

void UPlayerCombatComponent::TriggerBackstep()
{
	APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
	if (!PlayerBase) return;

	UDDSAbilitySystemComponent* ASC = PlayerBase->GetDDSAbilitySystemComponent();
	if (!ASC) return;

	// 컨트롤러가 보고 있는 방향의 반대 방향(뒤로)
	const FRotator ControlRot(0.f, PlayerBase->GetControlRotation().Yaw, 0.f);
	const FVector Backward = -FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X);

	ASC->SetLastDodgeInputDirection(Backward.GetSafeNormal());

	MY_LOG(LogTemp, Log, TEXT("TriggerBackstep: WorldDir=%s"), *Backward.ToString());

	// 실제 백스텝 GA는 블루프린트로 구현 예정
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
