// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#include "GameAbilitySystem/Effects/GE_WeaponEquip.h"
#include "GameAbilitySystem/DDSAttributeSet.h"
#include "DDSGameplayTags.h"

UGE_WeaponEquip::UGE_WeaponEquip()
{
	// Duration: Infinite (무기 해제할 때까지 유지)
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	// 스택 불가 (동시에 여러 무기 장착 안됨)
	StackingType = EGameplayEffectStackingType::None;
	
	// Period: 없음 (한번만 적용)
	Period = 0.0f;

	// ===== Blade Stats Modifiers =====

	// 1. BaseATK -> BaseAttack
	{
		FGameplayModifierInfo Modifier;
		FSetByCallerFloat Magnitude;
		Magnitude.DataTag = DDSGameplayTags::Weapon_SetByCaller_BaseATK;
		Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Magnitude);
		Modifier.ModifierOp = EGameplayModOp::Additive;
		Modifier.Attribute = UDDSAttributeSet::GetBaseAttackAttribute();
		Modifiers.Add(Modifier);
	}

	// 2. PowASR -> PhysicalASR (근력 보정)
	{
		FGameplayModifierInfo Modifier;
		FSetByCallerFloat Magnitude;
		Magnitude.DataTag = DDSGameplayTags::Weapon_SetByCaller_PowASR;
		Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Magnitude);
		Modifier.ModifierOp = EGameplayModOp::Additive;
		Modifier.Attribute = UDDSAttributeSet::GetPhysicalASRAttribute();
		Modifiers.Add(Modifier);
	}

	// 3. DexASR -> DexterityASR (기량 보정)
	{
		FGameplayModifierInfo Modifier;
		FSetByCallerFloat Magnitude;
		Magnitude.DataTag = DDSGameplayTags::Weapon_SetByCaller_DexASR;
		Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Magnitude);
		Modifier.ModifierOp = EGameplayModOp::Additive;
		Modifier.Attribute = UDDSAttributeSet::GetDexterityASRAttribute();
		Modifiers.Add(Modifier);
	}

	// 4. MgcASR -> MagicASR (마법 보정)
	{
		FGameplayModifierInfo Modifier;
		FSetByCallerFloat Magnitude;
		Magnitude.DataTag = DDSGameplayTags::Weapon_SetByCaller_MgcASR;
		Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Magnitude);
		Modifier.ModifierOp = EGameplayModOp::Additive;
		Modifier.Attribute = UDDSAttributeSet::GetMagicASRAttribute();
		Modifiers.Add(Modifier);
	}

	// 5. UseSTA -> AttackRequireStamina
	{
		FGameplayModifierInfo Modifier;
		FSetByCallerFloat Magnitude;
		Magnitude.DataTag = DDSGameplayTags::Weapon_SetByCaller_UseSTA;
		Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Magnitude);
		Modifier.ModifierOp = EGameplayModOp::Additive;
		Modifier.Attribute = UDDSAttributeSet::GetAttackRequireStaminaAttribute();
		Modifiers.Add(Modifier);
	}

	// Note: AttackSpeed, StanceATK, GripAttackSpeed, GripStanceATKTime are handled separately
	// in the combat system and don't need attribute modifiers
	// Plus 값들은 무시 (BaseATKPlus, PowASRPlus, DexASRPlus, MgcASRPlus)
}
