
#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "InventoryItemList.generated.h"

class UInventoryItemInstance;
class UItemStaticData;

USTRUCT(BlueprintType)
struct FInventoryItemEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	UInventoryItemInstance* ItemInstance = nullptr;
};

USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FInventoryItemEntry> Items;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FInventoryItemEntry, FInventoryList>(Items, DeltaParams, *this);
	}

	void AddItem(const TSubclassOf<UItemStaticData>& ItemClass);
	void AddItem(UInventoryItemInstance* Item);
	void RemoveItem(TSubclassOf<UItemStaticData> ItemClass);
	void RemoveItem(int32 Index);
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};