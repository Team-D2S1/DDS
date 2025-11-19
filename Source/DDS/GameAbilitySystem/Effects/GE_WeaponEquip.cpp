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


	// 9. UseSTA -> AttackRequireStamina
	{
		FGameplayModifierInfo Modifier;
		FSetByCallerFloat Magnitude;
		Magnitude.DataTag = DDSGameplayTags::Weapon_SetByCaller_UseSTA;
		Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Magnitude);
		Modifier.ModifierOp = EGameplayModOp::Additive;
		Modifier.Attribute = UDDSAttributeSet::GetAttackRequireStaminaAttribute();
		Modifiers.Add(Modifier);
	}

	// 10. AttackSpeed, StanceATK, GripAttackSpeed, GripStanceATKTime은
	// AttributeSet에 해당 Attribute가 있을 때 추가
	// 현재는 StrengthAR, DexterityAR, MagicAR 계열 + AttackRequireStamina 매핑 완료
	
	// 나중에 필요하면 추가:
	// - Attribute.Weapon.AttackSpeed
	// - Attribute.Weapon.StanceATK
	// 등의 Attribute를 DDSAttributeSet에 추가하고 여기에 Modifier 추가
}
