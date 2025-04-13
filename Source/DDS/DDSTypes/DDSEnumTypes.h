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