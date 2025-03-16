// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class DDS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySession(bool bWasSuccessful);
	
	
	virtual bool Initialize() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> SingleplayButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> MultiplayButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> OptionButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ExitButton;

private:
	UFUNCTION()
	void SingleplayButtonClicked();
	UFUNCTION()
	void MultiplayButtonClicked();
	UFUNCTION()
	void OptionButtonClicked();
	UFUNCTION()
	void ExitButtonClicked();

	
};
