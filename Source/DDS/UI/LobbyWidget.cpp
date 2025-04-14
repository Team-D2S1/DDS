// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "UI/LobbyWidget.h"

#include "OnlineSubsystem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Online/CoreOnline.h"
#include "GameState/LobbyGameState.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/LobbyPlayerController.h"
#include "Session/SessionSubsystem.h"
#include "Socket/SteamHelper.h"

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
	ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(UGameplayStatics::GetGameState(this));
	if(!LobbyGameState) return;
	
	if(ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(GetOwningPlayer()))
	{
		// Manager일 때, 플레이어 준비상태에 따라 Button 상태를 업데이트
		if(LobbyPC->bIsManager)
		{
			ReadyStartButton->SetIsEnabled(false);
			if(LobbyGameState->LobbyPlayerNum == LobbyGameState->ReadyPlayerNum)
			{
				ReadyStartButton->SetIsEnabled(true);
			}
		}

		// 스팀 플레이어 아이디와 아바타를 업데이트
		for(int i = 0; i < LobbyGameState->PlayerNetIds.Num(); i++)
		{
			// PlayerNetId 스트링을 UniqueNetId로 변경
			USessionSubsystem* SessionSubsystem = GetGameInstance()->GetSubsystem<USessionSubsystem>();
			TSharedPtr<const FUniqueNetId> PlayerUniqueNetId;
			if(SessionSubsystem)
			{
				PlayerUniqueNetId = SessionSubsystem->CreateUniqueIdFromString(LobbyGameState->PlayerNetIds[0]);
			}

			FString PlayerNickname;
			UTexture2D* PlayerAvatarImage = nullptr;
			if(PlayerUniqueNetId.IsValid())
			{
				IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
				if(OnlineSubsystem)
				{
					PlayerNickname = OnlineSubsystem->GetIdentityInterface()->GetPlayerNickname(*PlayerUniqueNetId);
				}
			}

			if(i == 0)
			{
				Player1Name->SetText(FText::FromString(PlayerNickname));

				FSlateBrush Brush;
				Brush.SetResourceObject(SteamHelper::GetSteamProfileImage());
				Brush.SetImageSize(UE::Slate::FDeprecateVector2DParameter(150.f, 150.f));
				Player1SteamImage->SetBrush(Brush);
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
