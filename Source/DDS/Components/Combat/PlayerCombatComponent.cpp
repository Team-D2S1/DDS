// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/PlayerCombatComponent.h"
#include "Character/Player/PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/Weapons/DDSPlayerWeapon.h"
#include "Kismet/KismetMathLibrary.h"

ADDSPlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<ADDSPlayerWeapon>(GetCharacterCarriedWeapon(InWeaponTag));
}

void UPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (FocusedObject)
	{
		APlayerBase* PlayerBase = GetOwningPawn<APlayerBase>();
		if (!PlayerBase) return;
		FVector TargetLocation = FocusedObject->GetActorLocation();
		// 플레이어가 적을 바라보도록
		FRotator PlayerLookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerBase->GetActorLocation(), TargetLocation);
		PlayerBase->SetActorRotation(FRotator(0.f, PlayerLookAtRotation.Yaw, 0.f));
		
		// 스프링암 각도 조정, 대상과 반대편이고 살짝 위로
		USpringArmComponent* SpringArm = PlayerBase->GetSpringArmComponent();
		if (!SpringArm) return;
		FVector PlayerLocation = PlayerBase->GetActorLocation();
		FRotator LookAtSpringArmRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);
		// 높게 
		LookAtSpringArmRotation.Pitch = -30.f;
		SpringArm->SetWorldRotation(LookAtSpringArmRotation);
		// 카메라 방향고정
		UCameraComponent* Cam = PlayerBase->GetCameraComponent();
		if (!Cam) return;
		FVector CameraLocation = Cam->GetComponentLocation();
		
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TargetLocation);
		Cam->SetWorldRotation(LookAtRotation);
	}
}
