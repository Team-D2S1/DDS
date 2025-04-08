// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class ULobbyWidget;
class UMainMenuWidget;
/**
 * 
 */
UCLASS()
class DDS_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	void GameStart();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LobbyWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UMainMenuWidget> MainMenuWidget;

	UPROPERTY()
	TObjectPtr<ULobbyWidget> LobbyWidget;
	
	UFUNCTION(Server, Reliable)
	void Server_GameStart();

protected:
	// MainMenuWidget 생성 후 할당, InputMode 변경
	void ShowMainMenuWidget();

	void ShowLobbyWidget();
	
public:
	UFUNCTION(Client, Reliable)
	void Client_PostLoginServer();
	
	FORCEINLINE UMainMenuWidget* GetMainMenuWidget() { return MainMenuWidget; }

	
};
