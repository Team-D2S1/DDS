// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/InventoryComponent.h"
#include "UI/DDSWidgetController.h"
#include "EquipWidgetController.generated.h"

class UInventoryComponent;



/**
 * 
 */
UCLASS()
class DDS_API UEquipWidgetController : public UDDSWidgetController
{
	GENERATED_BODY()

private:
	
public:

	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;

	
	UFUNCTION(BlueprintCallable, Category = "DDS|WidgetController|Equip")
	void OnInventoryUpdated();
	
	UPROPERTY(BlueprintAssignable, Category = "DDS|WidgetController|Equip")
	FInventroryUpdateEvent BP_UpdateUI;
private:



};
