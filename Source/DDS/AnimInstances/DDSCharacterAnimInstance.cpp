// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AnimInstances/DDSCharacterAnimInstance.h"

#include "ETC/CustomLog.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UDDSCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(TryGetPawnOwner());
	if(Character)
	{
		CharacterMovement = Character->GetCharacterMovement();
	}
}

void UDDSCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// Set Character
	if(!Character)
	{
		Character = Cast<ACharacter>(TryGetPawnOwner());
		if(Character)
		{
			CharacterMovement = Character->GetCharacterMovement();
		}
		else return;
	}

	// Set Speed
	FVector Velocity = Character->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	// Set Is In Air
	bIsInAir = CharacterMovement->IsFalling();

	// Set Is Accelerating
	bIsAccelerating = CharacterMovement->GetCurrentAcceleration().Size() > 0.f;

	// Calculate Character Rotation
	FRotator AimRotation = Character->GetBaseAimRotation();
	FRotator ActorRotation = Character->GetActorRotation();
	FRotator VelocityRotation = UKismetMathLibrary::MakeRotFromX(Velocity);

	// Set Direction
	FRotator DirectionDeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(ActorRotation, VelocityRotation);
	Direction = DirectionDeltaRotation.Yaw * (-1.f);

	// Set Offsets
	FRotator AimDeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
	YawOffset = AimDeltaRotation.Yaw;
	PitchOffset = AimDeltaRotation.Pitch;
	RollOffset = AimDeltaRotation.Roll;

	LateralSpeed = FVector::DotProduct(Character->GetActorRightVector(), Velocity);
	ForwardSpeed = FVector::DotProduct(Character->GetActorForwardVector(), Velocity);
}
