// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemList.h"
#include "Components/ActorComponent.h"
#include "Components/PawnExtensionComponentBase.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DDS_API UInventoryComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
	UFUNCTION(BlueprintCallable)
    void AddItem(TSubclassOf<UItemStaticData> ItemClass);
	UFUNCTION(BlueprintCallable)
	void AddItemInstance(UItemInstance* Item);
	void RemoveItem(TSubclassOf<UItemStaticData> ItemClass);
	void RemoveItem(int32 ItemID);
	void RemoveItemByIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	TArray<UItemInstance*> GetAllItems();

	/**
	 * 서버에서 조합된 무기를 추가한다.
	 * 재료아이템은 제거된다.
	 * @param WeaponItemClass 조합 아이템 기본 클래스 
	 * @param BladeItemID 칼날
	 * @param GripItemID 손잡이
	 * @param PommelItemID 무기장식
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_AddCraftedWeapon(TSubclassOf<UItemStaticData> WeaponItemClass,int32 BladeItemID, int32 GripItemID, int32 PommelItemID);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_EquipCraftedWeapon(int32 ItemID);
	
	UFUNCTION(BlueprintCallable)
	UItemInstance* GetItemByID(int32 ItemID);

	UFUNCTION(BlueprintCallable)
	FInventoryList& GetInventoryList();

	UFUNCTION(BlueprintPure)
	UItemInstance* GetRightWeaponItem() const { return RightWeapon; }
	UFUNCTION(BlueprintCallable)
	UItemInstance* GetArmorByIndex(int32 Index) const
	{
		switch (Index)
		{
		case 0: return Armor01;
		case 1: return Armor02;
		case 2: return Armor03;
		case 3: return Armor04;
		default: return nullptr;
		}
	}
	UFUNCTION(BlueprintPure)
	UItemInstance* GetArmor01() const { return Armor01; }
	UFUNCTION(BlueprintPure)
	UItemInstance* GetArmor02() const { return Armor02; }
	UFUNCTION(BlueprintPure)
	UItemInstance* GetArmor03() const { return Armor03; }
	UFUNCTION(BlueprintPure)
	UItemInstance* GetArmor04() const { return Armor04; }
	
	
	// UPROPERTY(BlueprintAssignable)
	// FInventoryItemEvent OnRepItemRemovedEvent;
	// UPROPERTY(BlueprintAssignable)
	// FInventoryItemEvent OnRepItemAddedEvent;
	// UPROPERTY(BlueprintAssignable)
	// FInventoryItemEvent OnRepItemChangedEvent;

	// UFUNCTION()
 // 	void OnRepItemAdded(int32 ItemID);
 //    UFUNCTION()
	// void OnRepItemRemoved(int32 ItemID);
	// UFUNCTION()
	// void OnRepItemChanged(int32 ItemID);

    UFUNCTION()
  	void OnRep_RightWeapon();
protected:

 	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	FInventoryList InventoryList;

 	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_RightWeapon)
	UItemInstance* RightWeapon;
	UPROPERTY(BlueprintReadOnly, Replicated)
	UItemInstance* Armor01;
	UPROPERTY(BlueprintReadOnly, Replicated)
	UItemInstance* Armor02;
	UPROPERTY(BlueprintReadOnly, Replicated)
	UItemInstance* Armor03;
	UPROPERTY(BlueprintReadOnly, Replicated)
	UItemInstance* Armor04;
public:	

};




