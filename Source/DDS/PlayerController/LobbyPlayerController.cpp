// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "LobbyPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "ETC/CustomLog.h"
#include "GameState/LobbyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/LobbyWidget.h"
#include "UI/MainMenuWidget.h"


ALobbyPlayerController::ALobbyPlayerController()
{
	bReplicates = true;
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsManager);
	DOREPLIFETIME(ThisClass, bIsReady);
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (World)
	{
		FString MapName = World->GetMapName(); 
		FString CleanName = FPackageName::GetShortName(MapName);
		if(CleanName == "MainMenu")
		{
			ShowMainMenuWidget();
		}else if (CleanName.Contains("MainMenu"))
		{
			ShowMainMenuWidget();
		}
		if(CleanName == "LobbyMenu")
		{
			ShowLobbyWidget();
		}else if (CleanName.Contains("LobbyMenu"))
		{
			ShowLobbyWidget();
		}
		
	}
}

void ALobbyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALobbyPlayerController::Client_PostLoginServer_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString MapName = World->GetMapName(); 
		FString CleanName = FPackageName::GetShortName(MapName);
		MY_LOG(LogTemp, Log, TEXT("현재 맵 이름: %s"), *CleanName);
	}
}

void ALobbyPlayerController::GameStart()
{
	Server_GameStart();
}

void ALobbyPlayerController::ReadyPlayer(bool bReady)
{
	Server_ReadyPlayer(bReady);
}

UTexture2D* ALobbyPlayerController::GetSteamProfileImage(int32 Width, int32 Height)
{
	UTexture2D* NewTexture = NewObject<UTexture2D>();

	// TODO
	// Do something...
	
	return NewTexture;
}

void ALobbyPlayerController::NetMulticast_UpdatePlayerInfo_Implementation(FLobbyPlayerInfo NewPlayerInfo)
{
	LobbyPlayerInfo.SteamID = NewPlayerInfo.SteamID;
	LobbyPlayerInfo.SteamImage = NewPlayerInfo.SteamImage;
	LobbyPlayerInfo.bIsReady = NewPlayerInfo.bIsReady;
}

void ALobbyPlayerController::Server_ReadyPlayer_Implementation(bool bReady)
{
	bIsReady = bReady;
	
	// Server에서 플레이어가 준비 완료 & 해제되었다고 Broadcast
	PlayerReadyDelegate.Broadcast(this, bIsReady);
}

void ALobbyPlayerController::OnRep_IsManagerChanged()
{
	// Manager은 자동으로 Ready상태가 된다.
	if(bIsManager)
	{
		bIsReady = true;
	}
	// Manager에서 해제되면 Ready가 풀림
	else
	{
		bIsReady = false;
	}
	
	if(LobbyWidget)
	{
		LobbyWidget->UpdateUI();
	}
}

void ALobbyPlayerController::ShowMainMenuWidget()
{
	if(MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);
		if(MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			ChangeUIInput();
		}
	}
}

void ALobbyPlayerController::ShowLobbyWidget()
{
	if(LobbyWidgetClass)
	{
		LobbyWidget = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
		if(LobbyWidget)
		{
			LobbyWidget->AddToViewport();
			ChangeUIInput();
		}
	}
}

void ALobbyPlayerController::ChangeUIInput()
{
	const FInputModeUIOnly InputModeData;
	SetInputMode(InputModeData);
	SetShowMouseCursor(true);
}

void ALobbyPlayerController::Server_GameStart_Implementation()
{
	MY_LOG(LogTemp, Error, TEXT("Server Travel to Test Level"));
	if(UWorld* World = GetWorld())
	{
		World->ServerTravel("/Game/Env_DDSProject/Maps/DDS_Project_WP");
	}
}
