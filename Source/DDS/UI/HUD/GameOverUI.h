// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DDSUserWidget.h"
#include "GameOverUI.generated.h"

class UButton;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class DDS_API UGameOverUI : public UDDSUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowGameOver();
	
	UFUNCTION(BlueprintCallable)
	void HideGameOver();
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UImage* GameOverBackground;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameOverText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* GameOverAnim;

	UFUNCTION()
	void OnFadeInFinished();
};
