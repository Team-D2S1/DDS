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
			spec.SourceObject = ASC->GetAvatarActor();
			spec.Level  = 1;
			ASC->GiveAbility(spec);
			rightWeaponItem = nullptr; // 무기 아이템을 nullptr로 설정(미리 null로 하면 Ability에서 접근 불가)
			}
		return;
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
//
