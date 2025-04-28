// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/DDSBaseAnimInstance.h"
#include "DDSCharacterAnimInstance.generated.h"

class UCharacterMovementComponent;
class AEntityBase;
/**
 * 
 */
UCLASS()
class DDS_API UDDSCharacterAnimInstance : public UDDSBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	ACharacter* Character;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
	UCharacterMovementComponent* CharacterMovement;
	
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float Direction;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float Speed;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float YawOffset;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float PitchOffset;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float RollOffset;
};
