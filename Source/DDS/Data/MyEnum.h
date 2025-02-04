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