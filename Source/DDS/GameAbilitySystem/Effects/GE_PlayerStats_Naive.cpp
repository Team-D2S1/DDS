#include "GameAbilitySystem/Effects/GE_PlayerStats_Naive.h"
#include "GameAbilitySystem/DDSAttributeSet.h"
#include "GameAbilitySystem/MMC/DerivedMMC.h"
#include "DDSGameplayTags.h"

UGE_PlayerStats_Naive::UGE_PlayerStats_Naive()
{
    DurationPolicy = EGameplayEffectDurationType::Infinite;
    
    // MMC를 주기적으로 재계산하여 실시간 반영
    Period = 0.1f; // 0.1초마다 재계산
    bExecutePeriodicEffectOnApplication = true; // 적용 즉시 실행

    // Effect를 식별할 수 있는 태그 추가
    InheritableGameplayEffectTags.Added.AddTag(DDSGameplayTags::GameplayEffect_PlayerStats);

    // Offense Attributes - MMC 사용
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
    
}
