// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "LobbyPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "UI/MainMenuWidget.h"


ALobbyPlayerController::ALobbyPlayerController()
{
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ShowMainMenuWidget();
}

void ALobbyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALobbyPlayerController::GameStart()
{
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

void ALobbyPlayerController::Server_GameStart_Implementation()
{
}
