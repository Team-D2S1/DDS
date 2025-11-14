// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "DerivedMMC.generated.h"

// Status Attributes MMC
UCLASS()
class UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxHealth();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};

UCLASS()
class UMMC_MaxStamina : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxStamina();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};

UCLASS()
class UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxMana();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};

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

// Defense Attributes MMC
UCLASS()
class UMMC_PhysicalDefense : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_PhysicalDefense();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};

UCLASS()
class UMMC_MagicDefense : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MagicDefense();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};

UCLASS()
class UMMC_PhysicalResist : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_PhysicalResist();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};

UCLASS()
class UMMC_MagicResist : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MagicResist();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
