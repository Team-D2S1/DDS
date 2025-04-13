// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "UI/LobbyWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ETC/CustomLog.h"
#include "GameState/LobbyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/LobbyPlayerController.h"

void ULobbyWidget::UpdatePlayerInfo(int32 PlayerIdx, UTexture2D* SteamImage, const FString& Name)
{
	
}

void ULobbyWidget::RemovePlayerInfo(int32 PlayerIdx)
{
	
}

bool ULobbyWidget::Initialize()
{
	if(!Super::Initialize()) return false;

	bIsReady = false;
	
	OptionButton->OnClicked.AddDynamic(this, &ThisClass::OptionButtonClicked);
	MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::MainMenuButtonClicked);

	return true;
}

void ULobbyWidget::UpdateUI()
{
	UpdateReadyStartButton();

	UpdatePlayer();
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
		if(LobbyPC->bIsManager)
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

void ULobbyWidget::UpdatePlayer()
{
	MY_LOG(LogTemp, Error, TEXT("Update Player 0"));
	ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(UGameplayStatics::GetGameState(this));
	if(!LobbyGameState) return;

	MY_LOG(LogTemp, Error, TEXT("Update Player 1"));
	// Server 입장
	if(ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(GetOwningPlayer()))
	{
		MY_LOG(LogTemp, Error, TEXT("Update Player 3"));

		if(LobbyPC->bIsManager)
		{
			MY_LOG(LogTemp, Error, TEXT("Update Player 4"));

			ReadyStartButton->SetIsEnabled(false);
			MY_LOG(LogTemp, Error, TEXT("LobbyPlayer : %d, ReadyPlayer : %d"), LobbyGameState->LobbyPlayerNum, LobbyGameState->ReadyPlayerNum);
			MY_LOG(LogTemp, Error, TEXT("Me Ready ? %d"), LobbyPC->bIsReady);
		
			if(LobbyGameState->LobbyPlayerNum == LobbyGameState->ReadyPlayerNum)
			{
				MY_LOG(LogTemp, Error, TEXT("Update Player 5"));
				ReadyStartButton->SetIsEnabled(true);
			}
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
	bIsReady = !bIsReady;
	
	ALobbyPlayerController* MyPC = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if(MyPC)
	{
		MyPC->ReadyPlayer(bIsReady);
	}
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
