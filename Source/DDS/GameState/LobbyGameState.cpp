// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "GameState/LobbyGameState.h"

#include "GameMode/LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/LobbyPlayerController.h"
#include "UI/LobbyWidget.h"


void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, LobbyPlayerNum);
	DOREPLIFETIME(ThisClass, ReadyPlayerNum);
}

void ALobbyGameState::Server_UpdatePlayerReady_Implementation(APlayerController* Controller, bool bIsReady)
{
	if(ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		for(auto PC : LobbyGameMode->PCs)
		{
			// PlayerInfo에서 bIsReady만 수정해서 다시 업데이트해준다
			if(Controller == PC)
			{
				FLobbyPlayerInfo NewPlayerInfo = Cast<ALobbyPlayerController>(PC)->LobbyPlayerInfo;
				NewPlayerInfo.bIsReady = bIsReady;
				
				Cast<ALobbyPlayerController>(PC)->NetMulticast_UpdatePlayerInfo(NewPlayerInfo);
			}
		}
	}

	ReadyPlayerNum = bIsReady ? ReadyPlayerNum++ : ReadyPlayerNum--;
}

void ALobbyGameState::OnRep_PlayerStateChanged()
{
	UWorld* World = GetWorld();
	if(World)
	{
		// Local PlayerController의 LobbyWidget을 최신화시킨다
		APlayerController* LocalPC = World->GetFirstPlayerController();
		if(LocalPC)
		{
			ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(LocalPC);
			if(LobbyPC)
			{
				LobbyPC->GetLobbyWidget()->UpdateUI();
			}
		}
	}
}
