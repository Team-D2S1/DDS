// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#include "DerivedMMC.h"

#include "GameplayEffectExecutionCalculation.h"
#include "DDS/GameAbilitySystem/DDSAttributeSet.h"

struct FDDSAttributeCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dexterity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Magic);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseAttack);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(StrengthAR);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DexterityAR);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicAR);

	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalASR);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DexterityASR);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicASR);

	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicPower);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Equip_AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Equip_MagicPower);

	FDDSAttributeCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Strength, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Dexterity, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Magic, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, BaseAttack, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, StrengthAR, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, DexterityAR, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, MagicAR, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, PhysicalASR, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, DexterityASR, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, MagicASR, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, AttackPower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, MagicPower, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Equip_AttackPower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Equip_MagicPower, Target, false);
	}
};

static const FDDSAttributeCapture& GetDDSAttributeCapture()
{
	static FDDSAttributeCapture AttributeCapture;
	return AttributeCapture;
}


// ===== Offense Attributes MMC =====

UMMC_AttackPower::UMMC_AttackPower()
{
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().StrengthDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().DexterityDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().BaseAttackDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().StrengthARDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().DexterityARDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().PhysicalASRDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().DexterityASRDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().Equip_AttackPowerDef);
}

float UMMC_AttackPower::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Strength = 0.f;
	float Dexterity = 0.f;
	float StrengthAR = 0.f;
	float DexterityAR = 0.f;
	float StrenghtASR = 0.f;
	float DexterityASR = 0.f;
	float EquipAttackPower = 0.f;
	float AttackPower = 0.f;
	float BaseATK = 0.f;

	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().StrengthDef, Spec, EvaluationParameters, Strength);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().DexterityDef, Spec, EvaluationParameters, Dexterity);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().BaseAttackDef, Spec, EvaluationParameters, BaseATK);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().StrengthARDef, Spec, EvaluationParameters, StrengthAR);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().DexterityARDef, Spec, EvaluationParameters, DexterityAR);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().PhysicalASRDef, Spec, EvaluationParameters, StrenghtASR);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().DexterityASRDef, Spec, EvaluationParameters, DexterityASR);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().Equip_AttackPowerDef, Spec, EvaluationParameters, EquipAttackPower);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().AttackPowerDef, Spec, EvaluationParameters, AttackPower);

	// BaseATK 최솟값 보장
	BaseATK = FMath::Max(BaseATK, 1.f);

	/*
	 * ATK=BaseATK*[1+{(ASR1)*AR1+{(ASR2)*AR2}]
	 * ASR2, AR2는 같은 속성에 두개의 스탯이 보정될때 추가
	 *
	 */
	
	// 공식: 물리 공격력 = BaseATK * [1 + ((PhysicalASR * StrengthAR) + (DexterityASR * DexterityAR))] + EquipAttackPower
	// 근력: 물리 공격력, 물리 방어력
	// 기량: 물리 공격력
	float CalculatedAttackPower = BaseATK * (1.f + ((StrenghtASR * StrengthAR) + (DexterityASR * DexterityAR))) + EquipAttackPower;

	return CalculatedAttackPower;
}

UMMC_MagicPower::UMMC_MagicPower()
{
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().MagicDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().BaseAttackDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().MagicARDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().MagicASRDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().MagicPowerDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().Equip_MagicPowerDef);
}

float UMMC_MagicPower::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Magic = 0.f;
	float MagicAR = 0.f;
	float MagicASR = 0.f;
	float EquipMagicPower = 0.f;
	float MagicPower = 0.f;
	float BaseATK = 0.f;

	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicDef, Spec, EvaluationParameters, Magic);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().BaseAttackDef, Spec, EvaluationParameters, BaseATK);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicARDef, Spec, EvaluationParameters, MagicAR);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicASRDef, Spec, EvaluationParameters, MagicASR);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().Equip_MagicPowerDef, Spec, EvaluationParameters, EquipMagicPower);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicPowerDef, Spec, EvaluationParameters, MagicPower);

	// BaseATK 최솟값 보장
	BaseATK = FMath::Max(BaseATK, 1.f);

	// 공식: 마법 공격력 = BaseATK * [1 + (MagicASR * MagicAR)] + EquipMagicPower
	// 마력: 마법 공격력, 마법 방어력
	float CalculatedMagicPower = BaseATK * (1.f + (MagicASR * MagicAR)) + EquipMagicPower;

	return CalculatedMagicPower;
}

