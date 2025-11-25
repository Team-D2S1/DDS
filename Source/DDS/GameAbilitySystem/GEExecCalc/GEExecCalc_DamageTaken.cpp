// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "GameAbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystemComponent.h"
#include "DDSGameplayTags.h"
#include "ETC/CustomLog.h"
#include "GameAbilitySystem/DDSAttributeSet.h"

struct FDDSDamageCapture
{
	// 느린방식의 FProperty 구하는 방식과 동일
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicPower);

	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResist);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResist);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken);
	FGameplayEffectAttributeCaptureDefinition AttackPowerTargetDef;
	
	FDDSDamageCapture()
	{
		// 느린 방식의 FGameplayEffectAttributeCaptureDefinition 와 같음
		// 마지막 false는 스냅샷 여부(false면 중간에 값이 바뀌어도 반영함)
		// 소스랑 타겟 둘 다 캡쳐해야 된다면, 매크로말고 수동으로 두개 정의해야 함.
		/*
		 *
		FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
		UDDSAttributeSet::StaticClass(),
		GET_MEMBER_NAME_CHECKED(UDDSAttributeSet, Health)
		);
	
		FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
		 AttackPowerProperty,
		 EGameplayEffectAttributeCaptureSource::Source,
		 false);
		*/
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, MagicPower, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, PhysicalDefense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, MagicDefense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, PhysicalResist, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, MagicResist, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, DamageTaken, Target, false);
	}
};

static const FDDSDamageCapture& GetDDSDamageCapture()
{
	static FDDSDamageCapture DamageCapture;
	return DamageCapture;
}


UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	/* 기본적인 캡쳐 방법. 느림!!*/
	// RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);

	RelevantAttributesToCapture.Add(GetDDSDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetDDSDamageCapture().MagicPowerDef);
	
	RelevantAttributesToCapture.Add(GetDDSDamageCapture().PhysicalDefenseDef);
	RelevantAttributesToCapture.Add(GetDDSDamageCapture().MagicDefenseDef);
	RelevantAttributesToCapture.Add(GetDDSDamageCapture().PhysicalResistDef);
	RelevantAttributesToCapture.Add(GetDDSDamageCapture().MagicResistDef);
	
	RelevantAttributesToCapture.Add(GetDDSDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
/*
	공격력/방어력<0.125→ 데미지 = 공격력×0.1

공격력/방어력<1→ 데미지 = 공격력×(19.2/49×(공격력/방어력−0.125)2+0.1)

공격력/방어력<2.5→ 데미지 = 공격력×(−0.4/3×(공격력/방어력−2.5)2+0.7)

공격력/방어력<8→ 데미지 = 공격력×(−0.8/121×(공격력/방어력−8)2+0.9)

공격력/방어력≥8→ 데미지 = 공격력×0.9
	*/

	auto GetDamage = [](float AttackPower, float DefensePower) -> float
	{
		
		if (DefensePower <= 0.f)
		{
			return AttackPower * 0.9f;
		}
		
		const float Ratio = AttackPower / DefensePower;
		if (Ratio < 0.125f)
		{
			return AttackPower * 0.1f;
		}
		else if (Ratio < 1.f)
		{
			return AttackPower * (19.2f / 49.f * (Ratio - 0.125f) * 2 + 0.1f);
		}
		else if (Ratio < 2.5f)
		{
			return AttackPower * (-0.4f / 3.f * (Ratio - 2.5f) * 2 + 0.7f);
		}
		else if (Ratio < 8.f)
		{
			return AttackPower * (-0.8f / 121.f * (Ratio - 8.f) * 2 + 0.9f);
		}
		else
		{
			return AttackPower * 0.9f;
		}
	};
	
	const FGameplayEffectSpec& EffectSpec= ExecutionParams.GetOwningSpec();

	MY_LOG(LogTemp, Log, TEXT("Execute DamageTaken Exec"));

	/*UDDSGameplayAbility::MakeGameplayEffectSpecHandle 참고 */
	// FGameplayEffectContextHandle ContextHandle = EffectSpec.GetContext();
	// ContextHandle.GetSourceObject();
	// ContextHandle.GetAbility();
	// ContextHandle.GetInstigator();
	// ContextHandle.GetEffectCauser();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	
	float SourceAttackPower = 0.f;
	float TargetDefensePower = 0.f;
	float TargetResistPower = 0.f;
	bool bIsPhysicalDamage = true;

	
	for (const TPair<FGameplayTag,float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(DDSGameplayTags::Shared_DamageType_Physical))
		{
			bIsPhysicalDamage = true;
			break;
		}
		if (TagMagnitude.Key.MatchesTagExact(DDSGameplayTags::Shared_DamageType_Magic))
		{
			bIsPhysicalDamage = false;
			break;
		}
	}

	if (bIsPhysicalDamage)
	{
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDDSDamageCapture().AttackPowerDef,
	    EvaluationParameters, SourceAttackPower);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDDSDamageCapture().PhysicalDefenseDef,
			EvaluationParameters, TargetDefensePower);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDDSDamageCapture().PhysicalResistDef,
			EvaluationParameters, TargetResistPower);
	}
	else
	{
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDDSDamageCapture().MagicPowerDef,
			EvaluationParameters, SourceAttackPower);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDDSDamageCapture().MagicDefenseDef,
			EvaluationParameters, TargetDefensePower);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDDSDamageCapture().MagicResistDef,
			EvaluationParameters, TargetResistPower);
	}

	// 계산식 적용
	const float CalculatedDamage = GetDamage(SourceAttackPower, TargetDefensePower);

	// MY_LOG(LogTemp, Log, TEXT("DamageTaken Calculation: AttackPower: %f, DefensePower: %f, ResistPower: %f, CalculatedDamage: %f"),
		// SourceAttackPower, TargetDefensePower, TargetResistPower, CalculatedDamage);
	// 경감률 적용
	const float FinalDamage = CalculatedDamage * (1.f - TargetResistPower / 100.f);

	// const float FinalDamage = 90.f;
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		GetDDSDamageCapture().DamageTakenDef.AttributeToCapture,
		EGameplayModOp::Override,
		FinalDamage
	));
}
