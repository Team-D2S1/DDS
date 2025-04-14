// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "LobbyGameMode.h"
#include "ETC/CustomLog.h"
#include "GameFramework/PlayerState.h"
#include "GameState/LobbyGameState.h"
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
}

void ALobbyGameMode::StartPlay()
{
	Super::StartPlay();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();

	// 플레이어 닉네임 가져오기.. 디버그용
	FString PlayerName = NewPlayer->GetPawn()->GetPlayerState()->GetPlayerName();
	MY_LOG(LogTemp, Error, TEXT("Player \"%s\" Login!"), *PlayerName);

	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(NewPlayer);
	if(LobbyPC)
	{
		LobbyPC->PlayerReadyDelegate.AddUniqueDynamic(this, &ThisClass::OnPlayerReady);
		
		// 접속한 PlayerController에 대한 작업
		LobbyPC->Client_PostLoginServer();
		PCs.Add(LobbyPC);
		
		LobbyGameState->LobbyPlayerNum = PCs.Num();

		// NetId 설정
		if(APlayerState* PlayerState = LobbyPC->GetPlayerState<APlayerState>())
		{
			if(PlayerState->GetUniqueId().IsValid())
			{
				LobbyGameState->LobbyPlayerNum = PCs.Num();
				
				TSharedPtr<const FUniqueNetId> UniqueNetId = PlayerState->GetUniqueId().GetUniqueNetId();
				if(UniqueNetId.IsValid())
				{
					FString PlayerUniqueNetId = UniqueNetId->ToString();

					LobbyGameState->PlayerNetIds.Add(PlayerUniqueNetId);

					MY_LOG(LogTemp, Log, TEXT("Steam Unique ID : %s"), *PlayerUniqueNetId);
				}
			}
		}
		
		// 플레이어가 1명이면 해당 플레이어를 방장으로
		if(PCs.Num() == 1)
		{
			LobbyPC->bIsManager = true;
			LobbyGameState->UpdatePlayerReady(LobbyPC, true);
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	MY_LOG(LogTemp, Error, TEXT("Player Logout!"));

	PCs.Remove(Cast<APlayerController>(Exiting));

	// 남아있는 플레이어가 1명이면 해당 플레이어를 방장으로
	if(PCs.Num() == 1)
	{
		if(ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PCs[0]))
		{
			LobbyPC->bIsManager = true;
			GetGameState<ALobbyGameState>()->UpdatePlayerReady(LobbyPC, true);
		}
	}
}

void ALobbyGameMode::OnPlayerReady(ALobbyPlayerController* PC, bool bIsReady)
{
	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if(LobbyGameState)
	{
		LobbyGameState->UpdatePlayerReady(PC, bIsReady);
	}
}
