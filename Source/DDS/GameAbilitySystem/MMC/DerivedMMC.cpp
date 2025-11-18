// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#include "DerivedMMC.h"

#include "GameplayEffectExecutionCalculation.h"
#include "DDS/GameAbilitySystem/DDSAttributeSet.h"

struct FDDSAttributeCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Vitality);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Endurance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dexterity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Magic);

	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthMax);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaminaMax);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ManaMax);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(StrengthAR);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DexterityAR);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicAR);

	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResist);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResist);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Equip_HealthMax);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Equip_StaminaMax);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Equip_ManaMax);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Equip_AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Equip_MagicPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Equip_PhysicalDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Equip_MagicDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Equip_PhysicalResist);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Equip_MagicResist);

	FDDSAttributeCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Vitality, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Endurance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Strength, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Dexterity, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Magic, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, HealthMax, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, StaminaMax, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, ManaMax, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, StrengthAR, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, DexterityAR, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, MagicAR, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, AttackPower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, MagicPower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, PhysicalDefense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, MagicDefense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, PhysicalResist, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, MagicResist, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Equip_HealthMax, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Equip_StaminaMax, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Equip_ManaMax, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Equip_AttackPower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Equip_MagicPower, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Equip_PhysicalDefense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Equip_MagicDefense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Equip_PhysicalResist, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Equip_MagicResist , Target,false);
	}
};

static const FDDSAttributeCapture& GetDDSAttributeCapture()
{
	static FDDSAttributeCapture AttributeCapture;
	return AttributeCapture;
}

// ===== Status Attributes MMC =====

UMMC_MaxHealth::UMMC_MaxHealth()
{
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().VitalityDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().HealthMaxDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().Equip_HealthMaxDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Vitality = 0.f;
	float EquipHealthMax = 0.f;
	float HealthMax = 0.f;

	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().VitalityDef, Spec, EvaluationParameters, Vitality);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().Equip_HealthMaxDef, Spec, EvaluationParameters, EquipHealthMax);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().HealthMaxDef, Spec, EvaluationParameters, HealthMax);
	
	// 공식: 체력(HP) = 100 + (Vitality * 10) + EquipHealthMax
	float CalculatedHealthMax = 100.f + (Vitality * 10.f) + EquipHealthMax;

	return CalculatedHealthMax;
}

UMMC_MaxStamina::UMMC_MaxStamina()
{
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().EnduranceDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().StaminaMaxDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().Equip_StaminaMaxDef);
}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Endurance = 0.f;
	float EquipStaminaMax = 0.f;
	float StaminaMax = 0.f;

	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().EnduranceDef, Spec, EvaluationParameters, Endurance);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().Equip_StaminaMaxDef, Spec, EvaluationParameters, EquipStaminaMax);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().StaminaMaxDef, Spec, EvaluationParameters, StaminaMax);
	
	// 공식: 스태미나 = 100 + (Endurance * 5) + EquipStaminaMax
	// 지구력: 스태미나, 장비중량(장비 스태미나 보너스)
	float CalculatedStaminaMax = 100.f + (Endurance * 5.f) + EquipStaminaMax;

	return CalculatedStaminaMax;
}

UMMC_MaxMana::UMMC_MaxMana()
{
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().MagicDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().ManaMaxDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().Equip_ManaMaxDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Magic = 0.f;
	float EquipManaMax = 0.f;
	float ManaMax = 0.f;

	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicDef, Spec, EvaluationParameters, Magic);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().Equip_ManaMaxDef, Spec, EvaluationParameters, EquipManaMax);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().ManaMaxDef, Spec, EvaluationParameters, ManaMax);
	
	// 공식: 마나 = 50 + (Magic * 8) + EquipManaMax
	float CalculatedManaMax = 50.f + (Magic * 8.f) + EquipManaMax;

	return CalculatedManaMax;
}

// ===== Offense Attributes MMC =====

UMMC_AttackPower::UMMC_AttackPower()
{
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().StrengthDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().DexterityDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().StrengthARDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().DexterityARDef);
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
	float EquipAttackPower = 0.f;
	float AttackPower = 0.f;

	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().StrengthDef, Spec, EvaluationParameters, Strength);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().DexterityDef, Spec, EvaluationParameters, Dexterity);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().StrengthARDef, Spec, EvaluationParameters, StrengthAR);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().DexterityARDef, Spec, EvaluationParameters, DexterityAR);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().Equip_AttackPowerDef, Spec, EvaluationParameters, EquipAttackPower);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().AttackPowerDef, Spec, EvaluationParameters, AttackPower);


	/*
	 * ATK=BaseATK*[1+{(ASR1+X)*AR1+{(ASR2+X)*AR2}]*(1+A)+Add 
	 * ASR2, AR2는 같은 속성에 두개의 스탯이 보정될때 추가
	 *
	 */
	
	// 공식: 물리 공격력 = (Strength * StrengthAR) + (Dexterity * DexterityAR) + EquipAttackPower
	// 근력: 물리 공격력, 물리 방어력
	// 기량: 물리 공격력
	float CalculatedAttackPower = (Strength * StrengthAR) + (Dexterity * DexterityAR) + EquipAttackPower;

	return CalculatedAttackPower;
}

UMMC_MagicPower::UMMC_MagicPower()
{
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().MagicDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().MagicARDef);
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
	float EquipMagicPower = 0.f;
	float MagicPower = 0.f;

	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicDef, Spec, EvaluationParameters, Magic);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicARDef, Spec, EvaluationParameters, MagicAR);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().Equip_MagicPowerDef, Spec, EvaluationParameters, EquipMagicPower);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicPowerDef, Spec, EvaluationParameters, MagicPower);
	
	// 공식: 마법 공격력 = (Magic * MagicAR) + EquipMagicPower
	// 지성: 마법 공격력, 마법 방어력
	float CalculatedMagicPower = (Magic * MagicAR) + EquipMagicPower;

	return CalculatedMagicPower;
}

// ===== Defense Attributes MMC =====

UMMC_PhysicalDefense::UMMC_PhysicalDefense()
{
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().StrengthDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().StrengthARDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().PhysicalDefenseDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().Equip_PhysicalDefenseDef);
}

float UMMC_PhysicalDefense::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Strength = 0.f;
	float StrengthAR = 0.f;
	float EquipPhysicalDefense = 0.f;
	float PhysicalDefense = 0.f;

	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().StrengthDef, Spec, EvaluationParameters, Strength);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().StrengthARDef, Spec, EvaluationParameters, StrengthAR);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().Equip_PhysicalDefenseDef, Spec, EvaluationParameters, EquipPhysicalDefense);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().PhysicalDefenseDef, Spec, EvaluationParameters, PhysicalDefense);
	
	// 공식: 물리 방어력 = (Strength * StrengthAR * 0.5) + EquipPhysicalDefense
	// 근력: 물리 공격력, 물리 방어력
	float CalculatedPhysicalDefense = (Strength * StrengthAR * 0.5f) + EquipPhysicalDefense;

	return CalculatedPhysicalDefense;
}

UMMC_MagicDefense::UMMC_MagicDefense()
{
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().MagicDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().MagicARDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().MagicDefenseDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().Equip_MagicDefenseDef);
}

float UMMC_MagicDefense::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Magic = 0.f;
	float MagicAR = 0.f;
	float EquipMagicDefense = 0.f;
	float MagicDefense = 0.f;

	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicDef, Spec, EvaluationParameters, Magic);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicARDef, Spec, EvaluationParameters, MagicAR);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().Equip_MagicDefenseDef, Spec, EvaluationParameters, EquipMagicDefense);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicDefenseDef, Spec, EvaluationParameters, MagicDefense);
	
	// 공식: 마법 방어력 = (Magic * MagicAR * 0.5) + EquipMagicDefense
	// 지성: 마법 공격력, 마법 방어력
	float CalculatedMagicDefense = (Magic * MagicAR * 0.5f) + EquipMagicDefense;

	return CalculatedMagicDefense;
}

UMMC_PhysicalResist::UMMC_PhysicalResist()
{
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().EnduranceDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().PhysicalResistDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().Equip_PhysicalResistDef);
}

float UMMC_PhysicalResist::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Endurance = 0.f;
	float EquipPhysicalResist = 0.f;
	float PhysicalResist = 0.f;

	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().EnduranceDef, Spec, EvaluationParameters, Endurance);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().Equip_PhysicalResistDef, Spec, EvaluationParameters, EquipPhysicalResist);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().PhysicalResistDef, Spec, EvaluationParameters, PhysicalResist);
	
	// 공식: 물리 저항력 = (Endurance * 0.3) + EquipPhysicalResist (최대 75% 제한은 GE에서 처리)
	float CalculatedPhysicalResist = (Endurance * 0.3f) + EquipPhysicalResist;

	return CalculatedPhysicalResist;
}

UMMC_MagicResist::UMMC_MagicResist()
{
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().MagicDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().MagicResistDef);
	RelevantAttributesToCapture.Add(GetDDSAttributeCapture().Equip_MagicResistDef);
}

float UMMC_MagicResist::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Magic = 0.f;
	float EquipMagicResist = 0.f;
	float MagicResist = 0.f;

	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicDef, Spec, EvaluationParameters, Magic);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().Equip_MagicResistDef, Spec, EvaluationParameters, EquipMagicResist);
	GetCapturedAttributeMagnitude(GetDDSAttributeCapture().MagicResistDef, Spec, EvaluationParameters, MagicResist);
	
	// 공식: 마법 저항력 = (Magic * 0.3) + EquipMagicResist (최대 75% 제한은 GE에서 처리)
	float CalculatedMagicResist = (Magic * 0.3f) + EquipMagicResist;

	return CalculatedMagicResist;
}
