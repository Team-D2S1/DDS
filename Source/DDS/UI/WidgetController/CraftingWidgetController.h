// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/InventoryItemList.h"
#include "DDSTypes/DDSEnumTypes.h"
#include "UI/DDSWidgetController.h"
#include "CraftingWidgetController.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnListItemClicked, int32, Index, TSubclassOf<UItemStaticData>, ItemClass, UInventoryItemInstance*, ItemInstance);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemSlotEvent, int32, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPartSlotClickedEvent, EPartType, PartType, int32, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryButtonClickEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdateEvent);

class UInventoryComponent;



 /**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DDS_API UCraftingWidgetController : public UDDSWidgetController
{
	GENERATED_BODY()
	
public:
	
	/* UDDSWidgetController Begin ~ */
	UFUNCTION(BlueprintCallable, Category = "DDS|WidgetController")
	virtual void BroadcastInitialValue() override;
	UFUNCTION(BlueprintCallable, Category = "DDS|WidgetController")
	virtual void BindCallbacksToDependencies() override;
    /* ~ UDDSWidgetController End */

	/**
	 * @brief OnItemAdded 후 블루프린트에 알리는 델리게이트
	 */
	UPROPERTY(BlueprintAssignable, Category = "DDS|WidgetController|Crafting")
	FInventoryItemEvent OnItemAddedEvent;

	/**
	 * @brief OnItemRemoved 후 블루프린트에 알리는 델리게이트
	 */
	UPROPERTY(BlueprintAssignable, Category = "DDS|WidgetController|Crafting")
	FInventoryItemEvent OnItemRemovedEvent;

	/**
	 * @brief 조합대 위의 아이템 슬롯 클릭 시 '곧바로' 함수
	 */
	UPROPERTY(BlueprintAssignable, Category = "DDS|WidgetController|Crafting")
	FOnPartSlotClickedEvent OnPartSlotClickedEvent;

	/**
	 * @brief 조합대 위의 아이템 슬롯 클릭 시 처리 후 호출되는 함수
	 */
	UPROPERTY(BlueprintAssignable, Category = "DDS|WidgetController|Crafting")
	FInventoryItemSlotEvent OnItemMovedToInventorySlotEvent;
	
	/**
	 * @brief 인벤토리 슬롯 클릭 시 처리 후  호출되는 함수
	 */
	UPROPERTY(BlueprintAssignable, Category = "DDS|WidgetController|Crafting")
	FInventoryItemSlotEvent OnItemMovedToPartSlotEvent;


	UPROPERTY(BlueprintAssignable, Category = "DDS|WidgetController|Crafting")
	FInventoryUpdateEvent OnInventoryUpdatedEvent;
	/**
	 * @brief (임시) 삭제 키 클릭시 호출되는 함수
	 * @deprecated 테스트용 임시 델리게이트임
	 */
	UPROPERTY(BlueprintAssignable, Category = "DDS|WidgetController|Crafting")
	FInventoryButtonClickEvent OnRemoveButtonClickedEvent;

	/* UI 콜백 함수 Begin ~ */
	
	UFUNCTION(BlueprintCallable, Category = "DDS|WidgetController|Crafting")
	void OnPartSlotClicked(EPartType PartType, int32 ItemId);	
	UFUNCTION(BlueprintCallable, Category = "DDS|WidgetController|Crafting")
	void OnItemSlotClicked(int32 ItemId);

	UFUNCTION(BlueprintCallable, Category = "DDS|WidgetController|Crafting")
	void OnRemoveButtonClicked();

	/* ~ UI 콜백 함수 End */

	/* UI 헬퍼 함수 Begin ~ */
	
	UFUNCTION(BlueprintCallable, Category= "DDS|WidgetController|Crafting")
	void RemoveItemInInventory(int32 ItemId);

	UFUNCTION(BlueprintCallable, Category = "DDS|WidgetController|Crafting")
	void InvokeItemMovedToInventorySlot(int32 ItemID);

	/* UI 헬퍼 함수 Begin ~ */
	
	// void OnWidgetClosed();
protected:
	UItemInstance* NativeGetItemByID(int32 ItemID);
	UFUNCTION(BlueprintCallable, Category = "DDS|WidgetController|Crafting", meta = (DisplayName = "GetItemByID", ExpandEnumAsExecs = "OutSuccessType"))
	UItemInstance* BP_GetItemByID(int32 ItemID,EDDSSuccessType& OutSuccessType);

	/**
	 * InvetoryComponent에 아이템이 추가되면 호출되는함수
	 * @param ItemID 
	 */
	UFUNCTION()
	void OnItemAdded(int32 ItemID);

	/**
	 * InvetoryComponent에 아이템이 제거되면 호출되는함수
	 * @param ItemID 
	 */
	UFUNCTION()
	void OnItemRemoved(int32 ItemID);
	
private:
	TMap<EPartType, int32> PartMap;
};

