// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class ULobbyWidget;
class UMainMenuWidget;


USTRUCT()
struct FLobbyPlayerInfo
{
	GENERATED_BODY()

	FLobbyPlayerInfo()
		: SteamImage(nullptr), SteamID(""), bIsReady(false) {}

	UPROPERTY()
	UTexture2D* SteamImage;

	UPROPERTY()
	FString SteamID;

	UPROPERTY()
	bool bIsReady;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerReadyDelegate, ALobbyPlayerController*, PlayerController, bool, bIsReady);

UCLASS()
class DDS_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void GameStart();

	void ReadyPlayer(bool bReady);
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_UpdatePlayerInfo(FLobbyPlayerInfo NewPlayerInfo);

	UTexture2D* GetSteamProfileImage(int32 Width, int32 Height);

	UPROPERTY()
	FPlayerReadyDelegate PlayerReadyDelegate;
	
	UPROPERTY(ReplicatedUsing = OnRep_IsManagerChanged)
	bool bIsManager = false;

	UPROPERTY(Replicated)
	bool bIsReady;
	
	UPROPERTY(Replicated)
	int32 LobbyPlayerIdx;

	UPROPERTY()
	FLobbyPlayerInfo LobbyPlayerInfo;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

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

	UFUNCTION(Server, Reliable)
	void Server_ReadyPlayer(bool bReady);

	UFUNCTION()
	void OnRep_IsManagerChanged();

protected:
	void ShowMainMenuWidget();

	void ShowLobbyWidget();

	void ChangeUIInput();
	
public:
	UFUNCTION(Client, Reliable)
	void Client_PostLoginServer();
	
	FORCEINLINE UMainMenuWidget* GetMainMenuWidget() { return MainMenuWidget; }
	FORCEINLINE ULobbyWidget* GetLobbyWidget() { return LobbyWidget; }
};
