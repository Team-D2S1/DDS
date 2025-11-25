// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "DerivedMMC.generated.h"

// Offense Attributes MMC
UCLASS()
class UMMC_AttackPower : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_AttackPower();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};

UCLASS()
class UMMC_MagicPower : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MagicPower();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};

