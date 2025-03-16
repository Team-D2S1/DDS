// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "LobbySession.generated.h"

/**
 * 
 */
UCLASS()
class DDS_API ALobbySession : public AGameSession
{
	GENERATED_BODY()
	
public:
	ALobbySession();
	
	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite) override;
	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer) override;
	
	void GameStart();
	
protected:
	FDelegateHandle OnRegisterPlayerDelegateHandle;
	FDelegateHandle OnUnregisterPlayerDelegateHandle;

	TArray<APlayerController*> PCs;

	void OnRegisterPlayerComplete(FName NameOfSession, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccessful);
	void OnUnregisterPlayerComplete(FName NameOfSession, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccessful);
};
