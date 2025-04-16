// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"


class ALobbyPlayerController;

UCLASS()
class DDS_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdatePlayerReady(ALobbyPlayerController* Controller, bool bIsReady);
	
	UPROPERTY(ReplicatedUsing = OnRep_PlayerStateChanged)
	int32 LobbyPlayerNum;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerStateChanged)
	int32 ReadyPlayerNum;

	UPROPERTY(Replicated)
	TArray<FString> PlayerNetIds;
	
protected:
	// 로비 플레이어의 수, 준비된 플레이어의 수가 변했을 때 호출
	UFUNCTION()
	void OnRep_PlayerStateChanged();
};
