// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "UI/WidgetController/CraftingWidgetController.h"
#include "Character/Player/DDSPlayerState.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Items/ItemInstance/ItemInstance.h"
#include "Components/Inventory/InventoryItemList.h"
#include "ETC/CustomLog.h"
#include "UI/HUD/DDSHUD.h"


void UCraftingWidgetController::BroadcastInitialValue()
{
 	if (UInventoryComponent* InvComp = GetInventoryComponent())
	{
 		for (const FInventoryItemEntry& ItemEntry : InvComp->GetInventoryList().Items)
 		{
 			if (!ItemEntry.ItemInstance)
 				continue;
 			MY_LOG(LogTemp, Log, TEXT("ItemID : %d, Name : %s"), ItemEntry.ItemInstance->GetItemId(), *ItemEntry.ItemInstance->GetItemName());
 			OnItemAddedEvent.Broadcast(ItemEntry.ItemInstance->GetItemId());
 		}
	}
}

void UCraftingWidgetController::BindCallbacksToDependencies()
{
	if (UInventoryComponent* InvComp = GetInventoryComponent())
	{
		MY_LOG(LogTemp, Log, TEXT("Binding InventoryComponent callbacks"));
		// InvComp->OnRepItemAddedEvent.AddDynamic(this, &UCraftingWidgetController::OnItemAdded);
		// InvComp->OnRepItemRemovedEvent.AddDynamic(this, &UCraftingWidgetController::OnItemRemoved);
	}
}
void UCraftingWidgetController::OnPartSlotClicked(EPartType PartType, int32 ItemId)
{
	MY_LOG(LogTemp, Log, TEXT("PartSlotClicked, ItemId : %d"), ItemId);
	OnPartSlotClickedEvent.Broadcast(PartType, ItemId);
	
	// if (ItemId == 0)
	// {
	// 	MY_LOG(LogTemp, Log, TEXT("ItemId is 0"));
	// 	return;
	// }
	// OnItemMovedToInventorySlotEvent.Broadcast(ItemId);
	OnInventoryUpdatedEvent.Broadcast();
}

void UCraftingWidgetController::OnItemSlotClicked(int32 ItemId)
{
	MY_LOG(LogTemp, Log, TEXT("ItemSlotClicked, ItemId : %d"), ItemId);
	OnItemMovedToPartSlotEvent.Broadcast(ItemId);
	
	OnInventoryUpdatedEvent.Broadcast();
}

void UCraftingWidgetController::OnRemoveButtonClicked()
{
	MY_LOG(LogTemp, Log, TEXT("RemoveButtonClicked"));
	OnRemoveButtonClickedEvent.Broadcast();

	OnInventoryUpdatedEvent.Broadcast();
}
// void UCraftingWidgetController::OnWidgetClosed()
// {
// 	
// }

void UCraftingWidgetController::RemoveItemInInventory(int32 ItemId)
{
	if (UInventoryComponent* InvComp = GetInventoryComponent())
	{
		InvComp->RemoveItem(ItemId);
	}

	OnInventoryUpdatedEvent.Broadcast();
}

void UCraftingWidgetController::InvokeItemMovedToInventorySlot(int32 ItemID)
{
	OnItemMovedToInventorySlotEvent.Broadcast(ItemID);

	OnInventoryUpdatedEvent.Broadcast(); 
}



UItemInstance* UCraftingWidgetController::NativeGetItemByID(int32 ItemID)
{
	if (UInventoryComponent* InvComp = GetInventoryComponent())
	{
		UItemInstance* Item = InvComp->GetItemByID(ItemID);
		if (Item)
		{
			
			return Item;
		}
	}
	return nullptr;
}

UItemInstance* UCraftingWidgetController::BP_GetItemByID(int32 ItemID, EDDSSuccessType& OutSuccessType)
{
	if (ItemID == 0)
	{
		OutSuccessType = EDDSSuccessType::Fail;
		MY_LOG(LogTemp, Warning, TEXT("ItemID is 0, returning nullptr"));
		return nullptr;
	}
	if (UItemInstance* result = NativeGetItemByID(ItemID))
	{
		OutSuccessType = EDDSSuccessType::Success;
		return result;
	}
	else
	{
		OutSuccessType = EDDSSuccessType::Fail;
		return nullptr;
	}
}



void UCraftingWidgetController::OnItemAdded(const int32 ItemID)
{
	if (UInventoryComponent* InvComp = GetInventoryComponent())
	{
		UItemInstance* Item = InvComp->GetItemByID(ItemID);
		if (Item)
		{
			OnItemAddedEvent.Broadcast(ItemID);
		}
	}

	OnInventoryUpdatedEvent.Broadcast();
}

void UCraftingWidgetController::OnItemRemoved(const int32 ItemID)
{
	MY_LOG(LogTemp, Log, TEXT("OnItemRemoved, ItemID : %d"), ItemID);
	if (UInventoryComponent* InvComp = GetInventoryComponent())
	{
		OnItemRemovedEvent.Broadcast(ItemID);
	}

	OnInventoryUpdatedEvent.Broadcast();
}

