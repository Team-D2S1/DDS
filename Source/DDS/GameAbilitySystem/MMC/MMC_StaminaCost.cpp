// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "MMC_StaminaCost.h"

#include "GameplayEffectExecutionCalculation.h"
#include "GameAbilitySystem/DDSAttributeSet.h"
#include "GameAbilitySystem/Abilities/DDSGameplayAbility.h"


struct FDDSCostCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackRequireStamina);
	
	FDDSCostCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, AttackRequireStamina, Target, false);
	}
};

static const FDDSCostCapture& GetDDSCostCapture()
{
	static FDDSCostCapture CostCapture;
	return CostCapture;
}

UMMC_StaminaCost::UMMC_StaminaCost()
{
	RelevantAttributesToCapture.Add(GetDDSCostCapture().AttackRequireStaminaDef);
}

float UMMC_StaminaCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UDDSGameplayAbility* Ability = Cast<UDDSGameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());
	if (!Ability)
	{
		return 0.f;
	}

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	float StaminaCost = 0.f;
	GetCapturedAttributeMagnitude(GetDDSCostCapture().AttackRequireStaminaDef, Spec, EvaluationParameters, StaminaCost);
	return StaminaCost;
}
