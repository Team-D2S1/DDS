// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "UI/LobbyWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PlayerController/LobbyPlayerController.h"

void ULobbyWidget::UpdatePlayerInfo(int32 PlayerIdx, UTexture2D* SteamImage, const FString& Name)
{
}

void ULobbyWidget::UpdateUI()
{
	UpdateReadyStartButton();
}

bool ULobbyWidget::Initialize()
{
	if(!Super::Initialize()) return false;

	OptionButton->OnClicked.AddDynamic(this, &ThisClass::OptionButtonClicked);
	MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::MainMenuButtonClicked);

	return true;
}

void ULobbyWidget::UpdateReadyStartButton()
{
	if(!GetOwningPlayer()) return;

	if(ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(GetOwningPlayer()))
	{
		// ReadyStart 버튼 텍스트 업데이트
		FString ReadyStartString = LobbyPC->bIsManager ? FString("Start") : FString("Ready");
		ReadyStartText->SetText(FText::FromString(ReadyStartString));

		// bIsManager 업데이트
		this->bIsManager = LobbyPC->bIsManager;
		if(bIsManager)
		{
			ReadyStartButton->OnClicked.Clear();
			ReadyStartButton->OnClicked.AddDynamic(this, &ThisClass::StartButtonClicked);
		}
		else
		{
			ReadyStartButton->OnClicked.Clear();
			ReadyStartButton->OnClicked.AddDynamic(this, &ThisClass::ReadyButtonClicked);
		}
	}
}

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateUI();
}

void ULobbyWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	MenuTearDown();
}

void ULobbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void ULobbyWidget::StartButtonClicked()
{
	if(ALobbyPlayerController* MyPC = Cast<ALobbyPlayerController>(GetOwningPlayer()))
	{
		MyPC->GameStart();
	}
}

void ULobbyWidget::ReadyButtonClicked()
{
}

void ULobbyWidget::OptionButtonClicked()
{
	
}

void ULobbyWidget::MainMenuButtonClicked()
{
	
}

void ULobbyWidget::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}
