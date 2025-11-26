// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDSUserWidget.generated.h"

class UMonsterUIComponent;
class UPlayerUIComponent;

// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseWidgetEvent);

/**
 * DDS 커스텀 UserWidget
 */
UCLASS(Blueprintable, BlueprintType)
class DDS_API UDDSUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
	
	// virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;


	UFUNCTION(BlueprintImplementableEvent)
	void BP_HandleInputAction(const FGameplayTag& InputTag);
protected:
	
	virtual void NativeOnInitialized() override;
	
	/**
	 * Widget이 설정되면 호출되는 Blueprint 이벤트
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnWidgetControllerSet();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnOwningPlayerUIComponentInitialized(UPlayerUIComponent* OwningPlayerUIComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnOwningMonsterUIComponentInitialized(UMonsterUIComponent* OwningMonsterUIComponent);

private:
	// bool bIsOpening = false;
public:
	UFUNCTION(BlueprintCallable)
	void InitMonsterCreatedWidget(AActor* OwningMonsterActor);
	
};
