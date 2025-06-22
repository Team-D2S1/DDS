// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/PlayerCombatComponent.h"

#include "Character/Player/PlayerBase.h"
#include "Components/Inventory/InventoryComponent.h"
#include "ETC/CustomLog.h"
#include "Items/Actor/DDSSimplePlayerWeapon.h"
#include "Items/ItemInstance/ItemInstance.h"


ADDSSimplePlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<ADDSSimplePlayerWeapon>(GetCharacterCarriedWeapon(InWeaponTag));
}

ADDSSimplePlayerWeapon* UPlayerCombatComponent::GetPlayerCurrentEquippedWeapon() const
{
	return Cast<ADDSSimplePlayerWeapon>(GetCurrentEquippedWeapon());
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
	if (NewWeapon == nullptr)
	{
		MY_LOG(LogTemp, Error, TEXT("NewWeapon is null in NotifyRightWeaponChanged"));
		return;
	}
	rightWeaponItem = NewWeapon;
	BP_SummonCraftedWeaponAndRegister(NewWeapon->GetItemId());
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
