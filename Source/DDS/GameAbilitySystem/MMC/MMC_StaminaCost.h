// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_StaminaCost.generated.h"

/**
 * 
 */
UCLASS()
class DDS_API UMMC_StaminaCost : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_StaminaCost();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
