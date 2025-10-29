#include "GameAbilitySystem/Effects/GE_PlayerStats_Naive.h"
#include "GameAbilitySystem/DDSAttributeSet.h"
#include "GameAbilitySystem/MMC/DerivedMMC.h"

UGE_PlayerStats_Naive::UGE_PlayerStats_Naive()
{
    DurationPolicy = EGameplayEffectDurationType::Infinite;

    // Status Attributes - MMC 사용
    FGameplayModifierInfo HealthMaxModifier;
    FCustomCalculationBasedFloat HealthMaxMagnitude;
    HealthMaxMagnitude.CalculationClassMagnitude = UMMC_HealthMax::StaticClass();
    HealthMaxModifier.ModifierMagnitude = HealthMaxMagnitude;
    HealthMaxModifier.ModifierOp = EGameplayModOp::Override;
    HealthMaxModifier.Attribute = UDDSAttributeSet::GetHealthMaxAttribute();
    Modifiers.Add(HealthMaxModifier);

    FGameplayModifierInfo StaminaMaxModifier;
    FCustomCalculationBasedFloat StaminaMaxMagnitude;
    StaminaMaxMagnitude.CalculationClassMagnitude = UMMC_StaminaMax::StaticClass();
    StaminaMaxModifier.ModifierMagnitude = StaminaMaxMagnitude;
    StaminaMaxModifier.ModifierOp = EGameplayModOp::Override;
    StaminaMaxModifier.Attribute = UDDSAttributeSet::GetStaminaMaxAttribute();
    Modifiers.Add(StaminaMaxModifier);

    FGameplayModifierInfo ManaMaxModifier;
    FCustomCalculationBasedFloat ManaMaxMagnitude;
    ManaMaxMagnitude.CalculationClassMagnitude = UMMC_ManaMax::StaticClass();
    ManaMaxModifier.ModifierMagnitude = ManaMaxMagnitude;
    ManaMaxModifier.ModifierOp = EGameplayModOp::Override;
    ManaMaxModifier.Attribute = UDDSAttributeSet::GetManaMaxAttribute();
    Modifiers.Add(ManaMaxModifier);

    // Offense Attributes - MMC 사용
    FGameplayModifierInfo AttackPowerModifier;
    FCustomCalculationBasedFloat AttackPowerMagnitude;
    AttackPowerMagnitude.CalculationClassMagnitude = UMMC_AttackPower::StaticClass();
    AttackPowerModifier.ModifierMagnitude = AttackPowerMagnitude;
    AttackPowerModifier.ModifierOp = EGameplayModOp::Override;
    AttackPowerModifier.Attribute = UDDSAttributeSet::GetAttackPowerAttribute();
    Modifiers.Add(AttackPowerModifier);

    FGameplayModifierInfo MagicPowerModifier;
    FCustomCalculationBasedFloat MagicPowerMagnitude;
    MagicPowerMagnitude.CalculationClassMagnitude = UMMC_MagicPower::StaticClass();
    MagicPowerModifier.ModifierMagnitude = MagicPowerMagnitude;
    MagicPowerModifier.ModifierOp = EGameplayModOp::Override;
    MagicPowerModifier.Attribute = UDDSAttributeSet::GetMagicPowerAttribute();
    Modifiers.Add(MagicPowerModifier);

    // Defense Attributes - MMC 사용
    FGameplayModifierInfo PhysicalDefenseModifier;
    FCustomCalculationBasedFloat PhysicalDefenseMagnitude;
    PhysicalDefenseMagnitude.CalculationClassMagnitude = UMMC_PhysicalDefense::StaticClass();
    PhysicalDefenseModifier.ModifierMagnitude = PhysicalDefenseMagnitude;
    PhysicalDefenseModifier.ModifierOp = EGameplayModOp::Override;
    PhysicalDefenseModifier.Attribute = UDDSAttributeSet::GetPhysicalDefenseAttribute();
    Modifiers.Add(PhysicalDefenseModifier);

    FGameplayModifierInfo MagicDefenseModifier;
    FCustomCalculationBasedFloat MagicDefenseMagnitude;
    MagicDefenseMagnitude.CalculationClassMagnitude = UMMC_MagicDefense::StaticClass();
    MagicDefenseModifier.ModifierMagnitude = MagicDefenseMagnitude;
    MagicDefenseModifier.ModifierOp = EGameplayModOp::Override;
    MagicDefenseModifier.Attribute = UDDSAttributeSet::GetMagicDefenseAttribute();
    Modifiers.Add(MagicDefenseModifier);

    FGameplayModifierInfo PhysicalResistModifier;
    FCustomCalculationBasedFloat PhysicalResistMagnitude;
    PhysicalResistMagnitude.CalculationClassMagnitude = UMMC_PhysicalResist::StaticClass();
    PhysicalResistModifier.ModifierMagnitude = PhysicalResistMagnitude;
    PhysicalResistModifier.ModifierOp = EGameplayModOp::Override;
    PhysicalResistModifier.Attribute = UDDSAttributeSet::GetPhysicalResistAttribute();
    Modifiers.Add(PhysicalResistModifier);

    FGameplayModifierInfo MagicResistModifier;
    FCustomCalculationBasedFloat MagicResistMagnitude;
    MagicResistMagnitude.CalculationClassMagnitude = UMMC_MagicResist::StaticClass();
    MagicResistModifier.ModifierMagnitude = MagicResistMagnitude;
    MagicResistModifier.ModifierOp = EGameplayModOp::Override;
    MagicResistModifier.Attribute = UDDSAttributeSet::GetMagicResistAttribute();
    Modifiers.Add(MagicResistModifier);
}
