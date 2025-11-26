// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DDSGameMode.generated.h"

class AMonsterSpawnManager;
/**
 * 
 */
UCLASS()
class DDS_API ADDSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<AMonsterSpawnManager> MonsterSpawnManager;

	void ResetAllMonsters();

protected:
	virtual void BeginPlay() override;
	
};
