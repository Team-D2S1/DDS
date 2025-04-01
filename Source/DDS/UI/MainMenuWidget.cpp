// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "MainMenuWidget.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Components/Button.h"
#include "ETC/CustomLog.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Session/SessionSubsystem.h"
#include "Socket/ClientSocket.h"

void UMainMenuWidget::OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccessful)
{
	if(!SessionSubsystem) return;
	MY_LOG(LogTemp, Error, TEXT("%d, %s"), SessionResult.Num(), bWasSuccessful ? *FString("Success") : *FString("Fail"));
	for(auto Result : SessionResult)
	{
		FString Value;
		if(Result.Session.SessionSettings.Get(FName("ProjectName"), Value))
		{
			if(Value == "DDS")
			{
				SessionSubsystem->JoinSession(Result);
				return;				
			}
		}
	}

	if(!bWasSuccessful || SessionResult.Num() == 0)
	{
		MultiplayButton->SetIsEnabled(true);
	}
}

void UMainMenuWidget::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if(PlayerController)
			{
				MY_LOG(LogTemp, Error, TEXT("Address : %s"), *Address);
				PlayerController->ClientTravel(Address, TRAVEL_Absolute);
			}
		}
	}
}

bool UMainMenuWidget::Initialize()
{
	if(!Super::Initialize()) return false;
	
	SingleplayButton->OnClicked.AddDynamic(this, &ThisClass::SingleplayButtonClicked);
	MultiplayButton->OnClicked.AddDynamic(this, &ThisClass::MultiplayButtonClicked);
	OptionButton->OnClicked.AddDynamic(this, &ThisClass::OptionButtonClicked);
	ExitButton->OnClicked.AddDynamic(this, &ThisClass::ExitButtonClicked);

	if(UGameInstance* GameInstance = GetGameInstance())
	{
		SessionSubsystem = GameInstance->GetSubsystem<USessionSubsystem>();
		if(SessionSubsystem)
		{
			SessionSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSession);
			SessionSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		}
	}
	return true;
}

void UMainMenuWidget::SingleplayButtonClicked()
{
	// TODO
	// 싱글플레이 모드로 들어간다 그러면 방 안만들고 어쩌고 저쩌고 안하고 바로 시작하기
}

void UMainMenuWidget::MultiplayButtonClicked()
{
	// TODO
	// 멀티플레이 모드로 들어간다 방 만들고 찾고 하는 기능 필요함

	MultiplayButton->SetIsEnabled(false);

	const FString LobbyName = "DDS Room";
	const FString IsPrivate = "false";
	const FString Password = "";

	FString ExtraInfo = LobbyName + "|" + IsPrivate + "|" + Password;
	FSocketReceivedData ReceivedData;

	UClientSocket* NewSocket = NewObject<UClientSocket>(this);
	if(NewSocket)
	{
		bool bIsLocal = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
		// 소켓 통신
		ReceivedData = NewSocket->CreateSocket("Client_CreateRoom", ExtraInfo, bIsLocal);
		// 소켓 종료
		NewSocket = nullptr;
	}
	
	if(ReceivedData.bExist)
	{
		ReceivedLobbyPort = ReceivedData.ReceivedData[0];

		// 방금 만든 세션을 찾아야 한다
		if(SessionSubsystem)
		{
			SessionSubsystem->FindSession(4, ReceivedLobbyPort);
		}
	}
}

void UMainMenuWidget::OptionButtonClicked()
{
	// TODO
	// 여러가지 옵션을 설정함
}

void UMainMenuWidget::ExitButtonClicked()
{
	// TODO
	// 게임 끌게
	APlayerController* PlayerController = GetOwningPlayer();
	if(PlayerController)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}
