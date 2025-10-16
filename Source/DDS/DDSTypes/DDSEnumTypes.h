// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	None,
	Normal,
	Epic,
	Boss,
};


UENUM(BlueprintType)
enum class EPlayerType : uint8
{
	None,
	Warrior,
	
};

UENUM(BlueprintType)
enum EPartType : uint8
{
	None 	UMETA(DisplayName = "None"),
	Blade,
	Handle,
	Pommel,
	Full,
};


UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	None,
	Common,
	Rare,
	Epic,
	Legendary,
};

UENUM()
enum class EDDSValidType : uint8
{
	Invalid,
	Valid,
};


UENUM()
enum class EDDSSuccessType : uint8
{
	Success,
	Fail
};

UENUM()
enum class EDDSConfirmType : uint8
{
	Yes,
	No
};

UENUM(BlueprintType)
enum class EWeaponNumberProperty : uint8
{
	AttackPower UMETA(DisplayName = "AttackPower"), // 공격력
	AttackSpeedCoefficient UMETA(DisplayName = "AttackSpeedCoefficient"), // 공격속도 보정
	GuardReductionRate UMETA(DisplayName = "GuardReductionRate"), // 방어력 감소율
	
	StrengthFactor UMETA(DisplayName = "StrengthFactor"), // 힘 보정치
	DexterityFactor UMETA(DisplayName = "DexterityFactor"), // 민첩 보정치
	FaithFactor UMETA(DisplayName = "FaithFactor"), // 신앙 보정치
	
};