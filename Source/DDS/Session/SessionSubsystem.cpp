// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "SessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSessionSettings.h"
#include "ETC/CustomLog.h"
#include "Socket/ClientSocket.h"

class UClientSocket;

USessionSubsystem::USessionSubsystem():
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if(Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}
}

void USessionSubsystem::CreateSession(int32 NumPublicConnections, FString Port)
{
	if(!SessionInterface.IsValid())
	{
		return;
	}
	
	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if(ExistingSession)
	{
		DestroySession();
	}

	// 델리게이트를 FDelegateHandle 안에 저장한다. 이후 삭제할 수 있도록..
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
	
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSettings->bIsDedicated = true;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = false;
	LastSessionSettings->bUseLobbiesIfAvailable = false;

	LastSessionSettings->Set(FName("PortNumber"), Port, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->Set(FName("ProjectName"), FString("DDS"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	if(!SessionInterface->CreateSession(0, NAME_GameSession, *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		MultiplayerOnCreateSessionComplete.Broadcast(false);
	}
}

void USessionSubsystem::FindSession(int32 MaxSearchResult, FString Port)
{
	if(!SessionInterface.IsValid())
	{
		return;
	}

	FindSessionCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResult;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSearch->QuerySettings.Set(FName("PortNumber"), Port, EOnlineComparisonOp::Equals);
	LastSessionSearch->QuerySettings.Set(FName("ProjectName"), FString("DDS"), EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if(!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void USessionSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if(!SessionInterface)
	{
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if(!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void USessionSubsystem::DestroySession()
{
	if(!SessionInterface)
	{
		MultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if(SessionInterface->DestroySession(NAME_GameSession))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		MultiplayerOnDestroySessionComplete.Broadcast(false);
	}
}

void USessionSubsystem::StartSession()
{
}

void USessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(SessionInterface.IsValid())
	{
		if(bWasSuccessful)
		{
			// Steam에 세션이 등록되었음
			const FString Port = FString::FromInt(GetWorld()->URL.Port);
			FString ExtraInfo = Port;
			FSocketReceivedData ReceivedData;
			
			UClientSocket* NewSocket = NewObject<UClientSocket>(this);
			if(NewSocket)
			{
				bool bIsLocal = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
				// 소켓 통신
				ReceivedData = NewSocket->CreateSocket("Dedicated_CreateRoomSuccess", ExtraInfo, bIsLocal);
				// 소켓 종료
				NewSocket = nullptr;
			}	
		}
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void USessionSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	MY_LOG(LogTemp, Error, TEXT("%d"), LastSessionSearch->SearchResults.Num());
	if(SessionInterface.IsValid())
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
	}

	if(LastSessionSearch->SearchResults.Num() <= 0)
	{
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, true);
}

void USessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(SessionInterface.IsValid())
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	MultiplayerOnJoinSessionComplete.Broadcast(Result);
}

void USessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}
	if(bWasSuccessful && bCreateSessionOnDestroy)
	{
	}
	MultiplayerOnDestroySessionComplete.Broadcast(true);
}

void USessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}
