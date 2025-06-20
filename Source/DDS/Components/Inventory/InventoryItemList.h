
#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "InventoryItemList.generated.h"

class UItemInstance;
class UItemStaticData;
struct FInventoryList;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemEvent, int32, ItemID);

USTRUCT(BlueprintType)
struct FInventoryItemEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	UItemInstance* ItemInstance = nullptr;

	UPROPERTY()
	int32 EntryID = 0;

	/** 자동으로 호출되는 함수들 */
	void PreReplicatedRemove(const struct FInventoryList& InArraySerializer);
	void PostReplicatedAdd(const struct FInventoryList& InArraySerializer);
	void PostReplicatedChange(const struct FInventoryList& InArraySerializer);
	
	bool operator==(const FInventoryItemEntry& Other) const;
};

USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FInventoryItemEntry> Items;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FInventoryItemEntry, FInventoryList>(Items, DeltaParams, *this);
	}

	void AddItem(const TSubclassOf<UItemStaticData>& ItemClass);
	void AddItem(UItemInstance* Item);
	void RemoveItem(TSubclassOf<UItemStaticData> ItemClass);
	void RemoveItem(int32 ItemID);
	void RemoveItemAt(int32 Index);

	FInventoryItemEvent OnRepItemRemoved;
	FInventoryItemEvent OnRepItemAdded;
	FInventoryItemEvent OnRepItemChanged;
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};