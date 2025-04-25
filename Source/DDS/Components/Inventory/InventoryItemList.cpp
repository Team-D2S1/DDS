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

bool FInventoryItemEntry::operator==(const FInventoryItemEntry& Other) const
{
	if (ItemInstance && Other.ItemInstance)
	{
		return ItemInstance->GetItemId() == Other.ItemInstance->GetItemId();
	}
	if (ItemInstance == Other.ItemInstance)
	{
		return true;
	}
	return false;
}

void FInventoryList::AddItem(const TSubclassOf<UItemStaticData>& InItemClass)
{
	FInventoryItemEntry& NewItem = Items.AddDefaulted_GetRef();
	NewItem.ItemInstance = NewObject<UInventoryItemInstance>();
	if (!NewItem.ItemInstance->Init(InItemClass))
	{
		return;
	}
	MarkItemDirty(NewItem);
}

void FInventoryList::AddItem(UInventoryItemInstance* Item)
{
	if (!Item)
	{
		return;
	}
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

void FInventoryList::RemoveItem(int32 ItemID)
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemInstance->GetItemId() == ItemID)
		{
			Items.RemoveAt(i);
			MarkArrayDirty();
			break;
		}
	}
}

void FInventoryList::RemoveItemAt(int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		Items.RemoveAt(Index);
		MarkArrayDirty();
	}
}
