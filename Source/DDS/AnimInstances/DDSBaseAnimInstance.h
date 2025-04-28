// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DDSBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DDS_API UDDSBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	ACharacter* Character;

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
