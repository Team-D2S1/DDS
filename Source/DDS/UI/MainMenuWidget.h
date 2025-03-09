// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	virtual bool Initialize() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> SingleplayButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> MultiplayButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> OptionButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ExitButton;
	
	UFUNCTION()
	void SingleplayButtonClicked();
	UFUNCTION()
	void MultiplayButtonClicked();
	UFUNCTION()
	void OptionButtonClicked();
	UFUNCTION()
	void ExitButtonClicked();
};
