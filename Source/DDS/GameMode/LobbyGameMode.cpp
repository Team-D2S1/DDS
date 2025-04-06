// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "LobbyGameMode.h"

#include "ETC/CustomLog.h"
#include <iostream>

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
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

	UWorld* World = GetWorld();
	if (World)
	{
		FString MapName = World->GetMapName(); // 내부 이름 (예: "UEDPIE_0_LobbyMenu")
		FString CleanName = FPackageName::GetShortName(MapName); // 깔끔한 이름 추출
		UE_LOG(LogTemp, Log, TEXT("현재 맵 이름: %s"), *CleanName);
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
