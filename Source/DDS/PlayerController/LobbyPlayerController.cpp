// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "LobbyPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "ETC/CustomLog.h"
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

void ALobbyPlayerController::Client_PostLoginServer_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString MapName = World->GetMapName(); 
		FString CleanName = FPackageName::GetShortName(MapName);
		UE_LOG(LogTemp, Log, TEXT("현재 맵 이름: %s"), *CleanName);
	}

	// 현재 위젯을 닫는다
	//MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
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
