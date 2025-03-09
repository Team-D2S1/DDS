// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "MainMenuPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "ETC/CustomLog.h"

AMainMenuPlayerController::AMainMenuPlayerController()
{
}

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ShowMainMenuWidget();
}

void AMainMenuPlayerController::ShowMainMenuWidget()
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

void AMainMenuPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

