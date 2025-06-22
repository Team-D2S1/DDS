// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/DDSWidgetController.h"
#include "EquipWidgetController.generated.h"

class UInventoryComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateUIEvent);
/**
 * 
 */
UCLASS()
class DDS_API UEquipWidgetController : public UDDSWidgetController
{
	GENERATED_BODY()

private:
	
public:

	UFUNCTION(BlueprintCallable, Category = "DDS|WidgetController|Equip")
	UInventoryComponent* GetInventoryComponent();


	UPROPERTY(BlueprintAssignable, Category = "DDS|WidgetController|Equip")
	FUpdateUIEvent BP_UpdateUIEvent;
private:



};
