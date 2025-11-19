// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_WeaponEquip.generated.h"

/**
 * 무기 장착 시 적용되는 GameplayEffect
 * SetByCaller를 통해 Blade와 Grip의 스탯을 동적으로 받아서 캐릭터에 적용합니다.
 * 
 * 사용되는 SetByCaller Tags:
 * - Weapon.SetByCaller.BaseATK : 기본 공격력
 * - Weapon.SetByCaller.BaseATKPlus : 공격력 증가량
 * - Weapon.SetByCaller.PowASR : 근력 보정
 * - Weapon.SetByCaller.PowASRPlus : 근력 보정 증가량
 * - Weapon.SetByCaller.DexASR : 기량 보정
 * - Weapon.SetByCaller.DexASRPlus : 기량 보정 증가량
 * - Weapon.SetByCaller.MgcASR : 마법 보정
 * - Weapon.SetByCaller.MgcASRPlus : 마법 보정 증가량
 * - Weapon.SetByCaller.UseSTA : 스태미나 소모량
 * - Weapon.SetByCaller.AttackSpeed : 공격 속도
 * - Weapon.SetByCaller.StanceATK : 강인도 감쇄
 * - Weapon.SetByCaller.GripAttackSpeed : 그립 공격 속도 배율
 * - Weapon.SetByCaller.GripStanceATKTime : 그립 강인도 시간
 */
UCLASS()
class DDS_API UGE_WeaponEquip : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_WeaponEquip();
};

