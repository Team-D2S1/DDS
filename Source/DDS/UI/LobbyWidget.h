// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"


class UTextBlock;
class UImage;
class UButton;

UCLASS()
class DDS_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdatePlayerInfo(int32 PlayerIdx, UTexture2D* SteamImage, const FString& Name);

	void UpdateUI();
	
protected:
	virtual bool Initialize() override;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	UImage* Player1SteamImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1Name;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Player2SteamImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2Name;
	
	
	UPROPERTY(meta = (BindWidget))
	UButton* ReadyStartButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ReadyStartText;
	
	UPROPERTY(meta = (BindWidget))
	UButton* OptionButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuButton;
	
private:
	bool bIsManager = false;

	int32 PlayerNum = 1;

	void UpdateReadyStartButton();

	UFUNCTION()
	void StartButtonClicked();

	UFUNCTION()
	void ReadyButtonClicked();

	UFUNCTION()
	void OptionButtonClicked();

	UFUNCTION()
	void MainMenuButtonClicked();
	
	void MenuTearDown();
	
};
