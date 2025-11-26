// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "GameMode/DDSGameMode.h"

#include "EngineUtils.h"
#include "Actor/MonsterSpawnManager.h"


void ADDSGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AMonsterSpawnManager> It(GetWorld()); It; ++It)
	{
		MonsterSpawnManager = *It;
		break;
	}
}

void ADDSGameMode::ResetAllMonsters()
{
	if(MonsterSpawnManager)
	{
		MonsterSpawnManager->RespawnDeadMonsters();
		MonsterSpawnManager->ResetAllMonsters();
	}
}
