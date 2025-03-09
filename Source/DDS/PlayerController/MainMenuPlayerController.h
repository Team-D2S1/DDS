// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/MainMenuWidget.h"
#include "MainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DDS_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainMenuPlayerController();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UMainMenuWidget> MainMenuWidget;

protected:
	// MainMenuWidget 생성 후 할당, InputMode 변경
	void ShowMainMenuWidget();
	
public:
	FORCEINLINE UMainMenuWidget* GetMainMenuWidget() { return MainMenuWidget; }
};
