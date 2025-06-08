#pragma once


#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
public:
	// UPROPERTY(Replicated,BlueprintReadOnly, EditDefaultsOnly,Category = "DDS|Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemStaticData> ItemClass;
private:
};
