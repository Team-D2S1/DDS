// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DDS_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerController();
	
	UFUNCTION(Server, Reliable)
	void Server_GameStart();

protected:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	// UUserWidget* LobbyWidget;

	void GameStart();

	
};
