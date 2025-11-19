// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#include "GameAbilitySystem/Effects/GE_ArmorEquip.h"

UGE_ArmorEquip::UGE_ArmorEquip()
{
	// Duration: Infinite (방어구 해제할 때까지 유지)
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	// 스택 가능 (여러 방어구 동시 장착)
	StackingType = EGameplayEffectStackingType::AggregateBySource;
	
	// Period: 없음 (한번만 적용)
	Period = 0.0f;
}

