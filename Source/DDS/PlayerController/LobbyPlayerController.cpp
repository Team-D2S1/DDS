// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "LobbyPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "ETC/CustomLog.h"
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
		}
		if(CleanName == "LobbyMenu")
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
		UE_LOG(LogTemp, Log, TEXT("현재 맵 이름: %s"), *CleanName);
	}
}

void ALobbyPlayerController::GameStart()
{
	Server_GameStart();
}

void ALobbyPlayerController::OnRep_IsManagerChanged()
{
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
			bShowMouseCursor = true;
			const FInputModeUIOnly UIInputMode;
			SetInputMode(UIInputMode);
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
			bShowMouseCursor = true;
			const FInputModeUIOnly UIInputMode;
			SetInputMode(UIInputMode);
		}
	}
}

void ALobbyPlayerController::Server_GameStart_Implementation()
{
	MY_LOG(LogTemp, Error, TEXT("Server Travel to Test Level"));
	if(UWorld* World = GetWorld())
	{
		World->ServerTravel("/Game/Maps/DasanMultiTestMap");
	}
}
