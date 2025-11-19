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
	// DOREPLIFETIME(UItemInstance, PommelItemInstance);
}

FName UItemInstance::GetItemTypeID() const
{
	if (ItemClass)
	{
		UItemStaticData* ItemData = ItemClass.GetDefaultObject();
		if (ItemData)
		{
			return ItemData->ItemTypeID;
		}
	}
	MY_ERROR_DISPLAY(TEXT("ItemClass is nullptr or ItemData is nullptr"));
	return FName();
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
		// FString PommelName = PommelItemInstance ? PommelItemInstance->GetItemName() : FString();
		// if (BladeName.IsEmpty() && GripName.IsEmpty() && PommelName.IsEmpty())
		if (BladeName.IsEmpty() && GripName.IsEmpty())
		{
			return ItemName; // 기본 아이템 이름 반환
		}
		// if (PommelName.IsEmpty())
		// {
		// 	FString FullName = FString::Printf(TEXT("장식되지 않은 %s %s"), *BladeName, *GripName);
		// 	return FullName;
		// }
		// FString FullName = FString::Printf(TEXT("%s으로 장식된 %s %s"), *PommelName, *BladeName, *GripName);
		FString FullName = FString::Printf(TEXT("%s %s"), *BladeName, *GripName);
		return FullName;
	}
	else
	{
		return ItemName;
	}
}

TSubclassOf<UGameplayEffect> UItemInstance::GetItemEffectClass() const
{
	if (ItemClass)
	{
		if (UItemStaticData* ItemData = ItemClass.GetDefaultObject())
		{
			return ItemData->ItemEffectClass;
		}
	}
	MY_ERROR_DISPLAY(TEXT("ItemClass is nullptr or ItemData is nullptr"));
	return nullptr;
}

void UItemInstance::SetBladeItemInstance(UItemInstance* NewBladeItemInstance)
{
	BladeItemInstance = NewBladeItemInstance;
}

void UItemInstance::SetGripItemInstance(UItemInstance* NewGripItemInstance)
{
	GripItemInstance = NewGripItemInstance;
}

FBladeData* UItemInstance::GetBladeData() const
{
	static UDataTable* BladeDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Blueprints/Player/Item/DT_Blade.DT_Blade"));

	if (BladeItemInstance)
	{
		return BladeItemInstance->GetBladeData();
	}
	
	if (!BladeDataTable)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to find Blade DataTable"));
		return nullptr;
	}

	FBladeData* BladeData = BladeDataTable->FindRow<FBladeData>(GetItemTypeID(), TEXT("UItemInstance::GetBladeData"));
	if (!BladeData)
	{
		MY_ERROR_DISPLAY(TEXT("Blade data not found in DataTable"));
		return nullptr;
	}

	return BladeData;
}

FGripData* UItemInstance::GetGripData() const
{
	static UDataTable* GripDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Blueprints/Player/Item/DT_Grip.DT_Grip"));

	if (GripItemInstance)
	{
		return GripItemInstance->GetGripData();
	}
	
	if (!GripDataTable)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to find Grip DataTable"));
		return nullptr;
	}
	

	FGripData* GripData = GripDataTable->FindRow<FGripData>(GetItemTypeID(), TEXT("UItemInstance::GetGripData"));
	if (!GripData)
	{
		MY_ERROR_DISPLAY(TEXT("Grip data not found in DataTable"));
		return nullptr;
	}

	return GripData;
}

// void UItemInstance::SetPommelItemInstance(UItemInstance* NewPommelItemInstance)
// {
// 	// Pommel 은 더 이상 사용하지 않음
// 	// PommelItemInstance = NewPommelItemInstance;
// }
