// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "ItemInstance.h"

#include "DDSGameplayTags.h"
#include "DDSTypes/DDSClassTypes.h"
#include "ETC/CustomLog.h"
#include "Net/UnrealNetwork.h"


UItemInstance::UItemInstance()
{
	
}

bool UItemInstance::Init(TSubclassOf<UItemStaticData> ItemStaticData)
{
	ItemClass = ItemStaticData;
	UItemStaticData* ItemData = ItemClass.GetDefaultObject();
	if (!ItemData)
	{
		MY_ERROR_DISPLAY(TEXT("ItemStaticData is nullptr, Initialization failed"));
		return false;
	}
	ItemId = GetUniqueID();
	this->ItemClass = ItemStaticData;
	ItemName = ItemData->ItemName;
	ItemDescription = ItemData->ItemDescription;
	ItemTypeTag = ItemData->ItemTypeTag;
	ItemTags = ItemData->ItemStaticTags;
	return true;
}

void UItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UItemInstance, ItemId);
	DOREPLIFETIME(UItemInstance, ItemClass);
	DOREPLIFETIME(UItemInstance, ItemName);
	DOREPLIFETIME(UItemInstance, ItemDescription);
	DOREPLIFETIME(UItemInstance, ItemTypeTag);
	DOREPLIFETIME(UItemInstance, ItemTags);

	DOREPLIFETIME(UItemInstance, BladeItemInstance);
	DOREPLIFETIME(UItemInstance, GripItemInstance);
	DOREPLIFETIME(UItemInstance, PommelItemInstance);
}


UPaperSprite* UItemInstance::GetItemIcon() const
{
	if (ItemClass)
	{
		UItemStaticData* ItemData = ItemClass.GetDefaultObject();
		if (ItemData)
		{
			return ItemData->ItemIcon;
		}
	}
	MY_ERROR_DISPLAY(TEXT("ItemClass is nullptr or ItemData is nullptr"));
	return nullptr;
}

FString UItemInstance::GetItemName() const
{
	if (ItemTypeTag.MatchesTag(DDSGameplayTags::Item_Weapon_Crafted))
	{
		FString BladeName = BladeItemInstance ? BladeItemInstance->GetItemName() : FString();
		FString GripName = GripItemInstance ? GripItemInstance->GetItemName() : FString();
		FString PommelName = PommelItemInstance ? PommelItemInstance->GetItemName() : FString();
		if (BladeName.IsEmpty() && GripName.IsEmpty() && PommelName.IsEmpty())
		{
			return ItemName; // 기본 아이템 이름 반환
		}
		if (PommelName.IsEmpty())
		{
			FString FullName = FString::Printf(TEXT("장식되지 않은 %s %s"), *BladeName, *GripName);
			return FullName;
		}
		FString FullName = FString::Printf(TEXT("%s으로 장식된 %s %s"), *PommelName, *BladeName, *GripName);
		return FullName;
	}
	else
	{
		return ItemName;
	}
}

void UItemInstance::SetBladeItemInstance(UItemInstance* NewBladeItemInstance)
{
	BladeItemInstance = NewBladeItemInstance;
}

void UItemInstance::SetGripItemInstance(UItemInstance* NewGripItemInstance)
{
	GripItemInstance = NewGripItemInstance;
}

void UItemInstance::SetPommelItemInstance(UItemInstance* NewPommelItemInstance)
{
 	PommelItemInstance = NewPommelItemInstance;
}
