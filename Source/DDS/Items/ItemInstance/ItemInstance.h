// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DDSTypes/DDSEnumTypes.h"
#include "Items/Actor/DDSCraftedPlayerWeapon.h"
#include "ItemInstance.generated.h"

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
	
	virtual bool IsSupportedForNetworking() const override {return true;};
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	
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

	UFUNCTION(BlueprintCallable)
	void SetBladeItemInstance(UItemInstance* NewBladeItemInstance);
	UFUNCTION(BlueprintCallable)
	void SetGripItemInstance(UItemInstance* NewGripItemInstance);
	UFUNCTION(BlueprintCallable)
	void SetPommelItemInstance(UItemInstance* NewPommelItemInstance);
	UFUNCTION(BlueprintCallable)
	UItemInstance* GetBladeItemInstance() const { return BladeItemInstance; }
	UFUNCTION(BlueprintCallable)
	UItemInstance* GetGripItemInstance() const { return GripItemInstance; }
	UFUNCTION(BlueprintCallable)
	UItemInstance* GetPommelItemInstance() const { return PommelItemInstance; }

	UFUNCTION(BlueprintCallable)
	bool IsValidCraftedWeapon() const
	{
		return BladeItemInstance != nullptr && GripItemInstance != nullptr && PommelItemInstance != nullptr;
	}
	
protected:
	UPROPERTY(Replicated)
	UItemInstance* BladeItemInstance;
	UPROPERTY(Replicated)
	UItemInstance* GripItemInstance;
	UPROPERTY(Replicated)
	UItemInstance* PommelItemInstance;
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
	
	
	// 게임플레이 태그
	UPROPERTY(Replicated) 
	FGameplayTagContainer ItemTags;
	
};
