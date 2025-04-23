#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGameTeam : uint8
{
	Neutral, //Everyone ignores this team
	Monster,
	Player,
}; 