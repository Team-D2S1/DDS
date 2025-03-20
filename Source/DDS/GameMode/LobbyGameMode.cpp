// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "LobbyGameMode.h"

#include "ETC/CustomLog.h"
#include <iostream>
#include "Session/LobbySession.h"

ALobbyGameMode::ALobbyGameMode()
{
	GameSessionClass = ALobbySession::StaticClass();

	bUseSeamlessTravel = true;
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if(UGameInstance* GameInstance = GetGameInstance())
	{
		// 작업
	}
}
