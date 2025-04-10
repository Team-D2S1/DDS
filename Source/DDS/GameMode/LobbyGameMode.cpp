// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "LobbyGameMode.h"

#include "ETC/CustomLog.h"
#include <iostream>

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/LobbyPlayerController.h"
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

	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(NewPlayer);
	if(LobbyPC)
	{
		LobbyPC->Client_PostLoginServer();
		PCs.Add(LobbyPC);
	}
	
	// 플레이어가 1명이면 자동 방장
	if(PCs.Num() == 1)
	{
		LobbyPC->bIsManager = true;
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	FString PlayerName = Exiting->GetPawn()->GetPlayerState()->GetPlayerName();
	MY_LOG(LogTemp, Error, TEXT("Player \"%s\" Logout!"), *PlayerName);

	PCs.Remove(Cast<APlayerController>(Exiting));

	// 남아있는 플레이어가 1명이면 자동 방장
	if(PCs.Num() == 1)
	{
		if(ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PCs[0]))
		{
			LobbyPC->bIsManager = true;
		}
	}
}
