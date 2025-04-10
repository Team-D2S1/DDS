// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "LobbySession.h"

#include "OnlineSubsystem.h"
#include "SessionSubsystem.h"
#include "ETC/CustomLog.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/OnlineSessionInterface.h"


ALobbySession::ALobbySession()
{
	
}

void ALobbySession::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite)
{
	Super::RegisterPlayer(NewPlayer, UniqueId, bWasFromInvite);

	MY_LOG(LogTemp, Error, TEXT("Player Login"));
	
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if(Session.IsValid())
		{
			PCs.Add(NewPlayer);

			OnRegisterPlayerDelegateHandle = Session->AddOnRegisterPlayersCompleteDelegate_Handle(
				FOnRegisterPlayersCompleteDelegate::CreateUObject(this, &ThisClass::OnRegisterPlayerComplete));

			if(!Session->RegisterPlayer(SessionName, *UniqueId, false))
			{
				Session->ClearOnRegisterPlayersCompleteDelegate_Handle(OnRegisterPlayerDelegateHandle);
				OnRegisterPlayerDelegateHandle.Reset();
			}
		}
	}
}

void ALobbySession::UnregisterPlayer(const APlayerController* ExitingPlayer)
{
	Super::UnregisterPlayer(ExitingPlayer);

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if(Session.IsValid())
		{
			for(int i = PCs.Num()-1; i >= 0; i--)
			{
				if(PCs[i] == ExitingPlayer)
				{
					PCs.RemoveAt(i);
				}
			}

			OnUnregisterPlayerDelegateHandle = Session->AddOnUnregisterPlayersCompleteDelegate_Handle(
				FOnUnregisterPlayersCompleteDelegate::CreateUObject(this, &ThisClass::OnUnregisterPlayerComplete));
		}
	}
}

void ALobbySession::GameStart()
{
	
}

void ALobbySession::BeginPlay()
{
	Super::BeginPlay();

	
	if(UGameInstance* GameInstance = GetGameInstance())
	{
		if(USessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<USessionSubsystem>())
		{
			SessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::ALobbySession::OnCreateSession);

			// 현재 세션이 생성되었는지 확인, 생성되었다면 Return
			IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
			if(OnlineSubsystem)
			{
				IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
				if(SessionPtr->GetNamedSession(NAME_GameSession) != nullptr)
				{
					return;
				}
			}

			// 현재 생성된 세션이 없다면 세션을 만든다
			if(IsRunningDedicatedServer())
			{
				FString PortNumber;
		
				if(UWorld* World = GetWorld())
				{
					PortNumber = FString::FromInt(World->URL.Port);
				}

				if(!PortNumber.IsEmpty())
				{
					MY_LOG(LogTemp, Log, TEXT("Lobby port : %s"), *PortNumber);
					SessionSubsystem->CreateSession(2, PortNumber);
				}
			}
		}
	}
	
	
}

void ALobbySession::OnRegisterPlayerComplete(FName NameOfSession, const TArray<FUniqueNetIdRef>& PlayerIds,
                                             bool bWasSuccessful)
{
	if(!bWasSuccessful)
	{
		MY_LOG(LogTemp, Error, TEXT("Failed to register player!!"));
		return;
	}

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if(Session.IsValid())
		{
			// TODO
			// 플레이어 등록 완료시 할 작업 작성
		}
	}
}

void ALobbySession::OnUnregisterPlayerComplete(FName NameOfSession, const TArray<FUniqueNetIdRef>& PlayerIds,
	bool bWasSuccessful)
{
	if(!bWasSuccessful)
	{
		MY_LOG(LogTemp, Error, TEXT("Failed to unregister player!!"));
		return;
	}
	
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if(Session.IsValid())
		{
			// TODO
			// 플레이어 해제 완료시 할 작업 작성 
		}
	}
}

void ALobbySession::OnCreateSession(bool bWasSuccessful)
{
	// 로비 맵 오픈
	UWorld* World = GetWorld();
	if(World)
	{
		World->ServerTravel("/Game/Maps/LobbyMenu");
	}
}
