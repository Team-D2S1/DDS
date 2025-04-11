// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"


UCLASS()
class DDS_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_UpdatePlayerReady(APlayerController* Controller, bool bIsReady);
	
	UPROPERTY(ReplicatedUsing = OnRep_PlayerStateChanged)
	int32 LobbyPlayerNum;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerStateChanged)
	int32 ReadyPlayerNum;
	
protected:
	UFUNCTION()
	void OnRep_PlayerStateChanged();
};
