// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "LobbyGameMode.h"

#include "ETC/CustomLog.h"
#include <iostream>

#include "GameFramework/PlayerState.h"
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

void ALobbyGameMode::StartPlay()
{
	Super::StartPlay();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	FString PlayerName = NewPlayer->GetPawn()->GetPlayerState()->GetPlayerName();
	MY_LOG(LogTemp, Error, TEXT("Player \"%s\" Login!"), *PlayerName);
}
