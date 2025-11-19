// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_ArmorEquip.generated.h"

/**
 * 방어구 장착 시 적용되는 GameplayEffect
 * SetByCaller를 통해 방어구 스탯을 동적으로 받아서 캐릭터에 적용합니다.
 * 
 * 사용 예정 SetByCaller Tags (확장 가능):
 * - Armor.SetByCaller.PhysicalDefense : 물리 방어력
 * - Armor.SetByCaller.MagicDefense : 마법 방어력
 * - Armor.SetByCaller.PhysicalResist : 물리 저항
 * - Armor.SetByCaller.MagicResist : 마법 저항
 * - Armor.SetByCaller.Weight : 무게
 */
UCLASS()
class DDS_API UGE_ArmorEquip : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_ArmorEquip();
};

