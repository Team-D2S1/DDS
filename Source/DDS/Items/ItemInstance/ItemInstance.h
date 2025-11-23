// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DataTable/BladeData.h"
#include "DataTable/GripData.h"
#include "DDSTypes/DDSEnumTypes.h"
#include "ItemInstance.generated.h"


class UGameplayEffect;
class UPaperSprite;
class UItemStaticData;
/**111
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DDS_API UItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
    UItemInstance();
	
	UFUNCTION(BlueprintCallable, Category = "DDS|Item")
	virtual bool Init(TSubclassOf<UItemStaticData> ItemClass);
	
	virtual bool IsSupportedForNetworking() const override {return true;}
	float GetItemLevel() const { return ItemLevel; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION(BlueprintPure, Category = "DDS|Item")
	FName GetItemTypeID() const;
	
	UFUNCTION(BlueprintPure, Category = "DDS|Item")
    int GetItemId() const { return ItemId; }
    UFUNCTION(BlueprintPure, Category = "DDS|Item")
    UPaperSprite* GetItemIcon() const;
	UFUNCTION(BlueprintPure, Category = "DDS|Item")
	FString GetItemName() const;
	UFUNCTION(BlueprintPure, Category = "DDS|Item")
	FString GetItemDescription() const { return ItemDescription; }
	UFUNCTION(BlueprintPure, Category = "DDS|Item")
	FGameplayTag GetItemTypeTag() const { return ItemTypeTag; }
	UFUNCTION(BlueprintPure, Category = "DDS|Item")
	FGameplayTagContainer GetItemTags() const { return ItemTags; }
	UFUNCTION(BlueprintPure, Category = "DDS|Item")
	TSubclassOf<UGameplayEffect> GetItemEffectClass() const;

	UFUNCTION(BlueprintPure, Category = "DDS|Item")
	TSubclassOf<UItemStaticData> GetItemClass() const { return ItemClass; }

	UFUNCTION(BlueprintCallable)
	void SetBladeItemInstance(UItemInstance* NewBladeItemInstance);
	UFUNCTION(BlueprintCallable)
	void SetGripItemInstance(UItemInstance* NewGripItemInstance);
	// Pommel 은 더 이상 사용하지 않음
	// UFUNCTION(BlueprintCallable)
	// void SetPommelItemInstance(UItemInstance* NewPommelItemInstance);
	UFUNCTION(BlueprintCallable)
	UItemInstance* GetBladeItemInstance() const { return BladeItemInstance; }
	UFUNCTION(BlueprintCallable)
	UItemInstance* GetGripItemInstance() const { return GripItemInstance; }
	// UFUNCTION(BlueprintCallable)
	// UItemInstance* GetPommelItemInstance() const { return PommelItemInstance; }

	UFUNCTION(BlueprintCallable)
	bool IsValidCraftedWeapon() const
	{
		return BladeItemInstance != nullptr && GripItemInstance != nullptr;// && PommelItemInstance != nullptr;
	}

	FBladeData* GetBladeData() const;
	
	FGripData* GetGripData() const;
	
protected:
	// 만약 기존에 복제되지 않은 아이템 인스턴스라면, InventoryComponent의 ReplicateSubobjects에 수정해줘야함.
	
	UPROPERTY(Replicated)
	UItemInstance* BladeItemInstance;
	UPROPERTY(Replicated)
	UItemInstance* GripItemInstance;
	// UPROPERTY(Replicated)
	// UItemInstance* PommelItemInstance;
private:
	UPROPERTY(Replicated,BlueprintReadOnly, EditDefaultsOnly,Category = "DDS|Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemStaticData> ItemClass;

	UPROPERTY(Replicated, VisibleAnywhere,Category = "DDS|Item")
	int32 ItemId;
	
	UPROPERTY(Replicated, EditDefaultsOnly,BlueprintReadOnly,Category = "DDS|Item", meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	UPROPERTY(Replicated, EditDefaultsOnly,BlueprintReadOnly,Category = "DDS|Item", meta = (AllowPrivateAccess = "true"))
	FString ItemDescription;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "DDS|Item", meta = (AllowPrivateAccess = "true"))
	FGameplayTag ItemTypeTag;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "DDS|Item", meta = (AllowPrivateAccess = "true"))
	int32 ItemLevel = 0;

	UFUNCTION(BlueprintCallable, Category = "DDS|Item",meta = (DisplayName = "Get Blade Data",ExpandEnumAsExecs = "OutSuccessType"))
	FBladeData BP_GetBladeData(EDDSSuccessType& OutSuccessType) const
	{
		FBladeData* BladeData = GetBladeData();
		if (BladeData)
		{
			OutSuccessType = EDDSSuccessType::Success;
			return *BladeData;
		}
		else
		{
			OutSuccessType = EDDSSuccessType::Fail;
			return FBladeData();
		}
	}

	UFUNCTION(BlueprintCallable, Category = "DDS|Item",meta = (DisplayName = "Get Grip Data",ExpandEnumAsExecs = "OutSuccessType"))
	FGripData BP_GetGripData(EDDSSuccessType& OutSuccessType) const
	{
		FGripData* GripData = GetGripData();
		if (GripData)
		{
			OutSuccessType = EDDSSuccessType::Success;
			return *GripData;
		}
		else
		{
			OutSuccessType = EDDSSuccessType::Fail;
			return FGripData();
		}
	}
	

	
	
	// 게임플레이 태그
	UPROPERTY(Replicated) 
	FGameplayTagContainer ItemTags;
	
};
