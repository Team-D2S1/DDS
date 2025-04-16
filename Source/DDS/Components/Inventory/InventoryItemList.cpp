#include "InventoryItemList.h"
#include "InventoryItemInstance.h"
#include "DDSTypes/DDSClassTypes.h"
// #include "Net/Serialization/FastArraySerializer.h"


void FInventoryItemEntry::PreReplicatedRemove(const struct FInventoryList& InArraySerializer)
{
	InArraySerializer.OnRepItemAdded.Broadcast(ItemInstance->GetItemId());
}

void FInventoryItemEntry::PostReplicatedAdd(const struct FInventoryList& InArraySerializer)
{
	InArraySerializer.OnRepItemRemoved.Broadcast(ItemInstance->GetItemId());
}

void FInventoryItemEntry::PostReplicatedChange(const struct FInventoryList& InArraySerializer)
{
	InArraySerializer.OnRepItemRemoved.Broadcast(ItemInstance->GetItemId());
}

void FInventoryList::AddItem(const TSubclassOf<UItemStaticData>& InItemClass)
{
	FInventoryItemEntry& NewItem = Items.AddDefaulted_GetRef();
	NewItem.ItemInstance = NewObject<UInventoryItemInstance>();
	NewItem.ItemInstance->Init(InItemClass);
	MarkItemDirty(NewItem);
}

void FInventoryList::AddItem(UInventoryItemInstance* Item)
{
	FInventoryItemEntry& NewItem = Items.AddDefaulted_GetRef();
	NewItem.ItemInstance = Item;
	MarkItemDirty(NewItem);
}

void FInventoryList::RemoveItem(TSubclassOf<UItemStaticData> InItemClass)
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemInstance->IsA(InItemClass))
		{
			Items.RemoveAt(i);
			MarkArrayDirty();
			break;
		}
	}
}

void FInventoryList::RemoveItem(int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		Items.RemoveAt(Index);
		MarkArrayDirty();
	}
}
