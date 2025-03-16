// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "LobbySession.h"

#include "OnlineSubsystem.h"
#include "ETC/CustomLog.h"
#include "Interfaces/OnlineSessionInterface.h"


ALobbySession::ALobbySession()
{
	
}

void ALobbySession::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite)
{
	Super::RegisterPlayer(NewPlayer, UniqueId, bWasFromInvite);

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
			for(int i = PCs.Num()-1; i <= 0; i--)
			{
				if(PCs[i] == ExitingPlayer)
				{
					PCs.Remove(PCs[i]);
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
