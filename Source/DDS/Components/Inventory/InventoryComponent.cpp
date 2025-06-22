// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Inventory/InventoryComponent.h"

#include "AbilitySystemComponent.h"
#include "Character/Player/DDSPlayerState.h"
#include "Character/Player/PlayerBase.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "Engine/ActorChannel.h"
#include "Items/ItemInstance/ItemInstance.h"
#include "ETC/CustomLog.h"
#include "Kismet/KismetTextLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	// InventoryList.OnRepItemAdded.AddDynamic(this, &UInventoryComponent::OnRepItemAdded);
	// InventoryList.OnRepItemRemoved.AddDynamic(this, &UInventoryComponent::OnRepItemRemoved);
	// InventoryList.OnRepItemChanged.AddDynamic(this, &UInventoryComponent::OnRepItemChanged);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, InventoryList);
	DOREPLIFETIME(UInventoryComponent, RightWeapon);
	DOREPLIFETIME(UInventoryComponent, Armor01);
	DOREPLIFETIME(UInventoryComponent, Armor02);
	DOREPLIFETIME(UInventoryComponent, Armor03);
	DOREPLIFETIME(UInventoryComponent, Armor04);
}

bool UInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	auto RepObj = [&](UItemInstance* Obj)
	{
		if (Obj) bWroteSomething |= Channel->ReplicateSubobject(Obj, *Bunch, *RepFlags);
	};
	
	for (FInventoryItemEntry& Item : InventoryList.Items)
	{
		RepObj(Item.ItemInstance);
	}

	RepObj(RightWeapon);
	if (RightWeapon)
	{
		RepObj(RightWeapon->GetBladeItemInstance());
		RepObj(RightWeapon->GetGripItemInstance());
		RepObj(RightWeapon->GetPommelItemInstance());
	}
	RepObj(Armor01);
	RepObj(Armor02);
	RepObj(Armor03);
	RepObj(Armor04);
	
	return bWroteSomething;
}

void UInventoryComponent::AddItem(TSubclassOf<UItemStaticData> ItemClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItem(ItemClass);
	}
}

void UInventoryComponent::AddItemInstance(UItemInstance* Item)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.AddItem(Item);
	}
}

void UInventoryComponent::RemoveItem(TSubclassOf<UItemStaticData> ItemClass)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItem(ItemClass);
	}
}

void UInventoryComponent::RemoveItem(int32 ItemID)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItem(ItemID);
	}
}

void UInventoryComponent::RemoveItemByIndex(int32 Index)
{
	if (GetOwner()->HasAuthority())
	{
		InventoryList.RemoveItemAt(Index);
	}
}

TArray<UItemInstance*> UInventoryComponent::GetAllItems()
{
	TArray<UItemInstance*> ItemInstances;
	for (const FInventoryItemEntry& Item : InventoryList.Items)
	{
		ItemInstances.Add(Item.ItemInstance);
	}
	return ItemInstances;
}

void UInventoryComponent::Server_AddCraftedWeapon_Implementation(TSubclassOf<UItemStaticData> WeaponItemClass,
                                                                 int32 BladeItemID, int32 GripItemID, int32 PommelItemID)
{
	if (GetOwner()->HasAuthority())
	{
		MY_LOG(LogTemp, Log, TEXT("Server_AddCraftedWeapon called. BladeItemID: %d, GripItemID: %d, PommelItemID: %d")	,BladeItemID, GripItemID, PommelItemID);
		UItemInstance* WeaponItemInstance = NewObject<UItemInstance>();
		if (WeaponItemInstance->Init(WeaponItemClass))
		{
			UItemInstance* BladeItemInstance = GetItemByID(BladeItemID);
			UItemInstance* GripItemInstance = GetItemByID(GripItemID);
			UItemInstance* PommelItemInstance = GetItemByID(PommelItemID);
			if (!(BladeItemInstance && GripItemInstance && PommelItemInstance))
			{
				MY_ERROR_DISPLAY_NET(true, TEXT("One or more item instances are null. Blade: %d, Grip: %d, Pommel: %d"),
					BladeItemID, GripItemID, PommelItemID);
				return;
			}
			WeaponItemInstance->SetBladeItemInstance(BladeItemInstance);
			WeaponItemInstance->SetGripItemInstance(GripItemInstance);
			WeaponItemInstance->SetPommelItemInstance(PommelItemInstance);
			AddItemInstance(WeaponItemInstance);
			
			RemoveItem(BladeItemID);
			RemoveItem(GripItemID);
			RemoveItem(PommelItemID);
		}
	}
}


void UInventoryComponent::Server_EquipCraftedWeapon_Implementation(int32 ItemID)
{
	bool bIsServer = GetOwner()->HasAuthority();
	MY_LOG(LogTemp, Log, TEXT("[%s] Server_EquipCraftedWeapon called with ItemID: %d"),
		bIsServer ? TEXT("Server") : TEXT("Client"), ItemID);
	if (ItemID == 0)
	{
		// 해제만 진행
		RightWeapon = nullptr;
	}
	else
	{
		RightWeapon = GetItemByID(ItemID);
		if (GetOwner()->HasAuthority())
		{
			OnRep_RightWeapon();
		}
	}
}

UItemInstance* UInventoryComponent::GetItemByID(const int32 ItemID)
{
	for (const FInventoryItemEntry& Item : InventoryList.Items)
	{
		if (Item.ItemInstance->GetItemId() == ItemID)
		{
			return Item.ItemInstance;
		}
	}
	bool bIsServer = GetOwner()->HasAuthority();
	MY_ERROR_DISPLAY_NET(bIsServer,TEXT("Item with ID %d not found"), ItemID);
	return nullptr;
}

FInventoryList& UInventoryComponent::GetInventoryList()
{
	return InventoryList;
}

// void UInventoryComponent::OnRepItemAdded(int32 ItemID)
// {
// 	bool bIsServer = GetOwner()->HasAuthority();
// 	MY_LOG(LogTemp, Log, TEXT("[%s] OnRepItemAdded called for ItemID: %d"),
// 		bIsServer ? TEXT("Server") : TEXT("Client"), ItemID);
// 	if (bIsServer)
// 	{
// 		OnRepItemAddedEvent.Broadcast(ItemID);
// 	}
// 	else
// 	{
//
// 	}
// 	
// }
//
// void UInventoryComponent::OnRepItemRemoved(int32 ItemID)
// {
// 	bool bIsServer = GetOwner()->HasAuthority();
// 	MY_LOG(LogTemp, Log, TEXT("[%s] OnRepItemRemoved called for ItemID: %d"),
// 		bIsServer ? TEXT("Server") : TEXT("Client"), ItemID);
// 	if (bIsServer)
// 	{
// 		OnRepItemRemovedEvent.Broadcast(ItemID);
// 	}
// 	else
// 	{
// 		
// 	}
// 	
// }
//
// void UInventoryComponent::OnRepItemChanged(int32 ItemID)
// {
// 	bool bIsServer = GetOwner()->HasAuthority();
// 	if (bIsServer)
// 	{
// 		OnRepItemChangedEvent.Broadcast(ItemID);
// 	}
// 	else
// 	{
//
// 	}
// }

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UInventoryComponent::OnRep_InventoryList()
{
	InventoryUpdatedEvent.Broadcast();
}

void UInventoryComponent::OnRep_RightWeapon()
{
	// CombatComponent의 현재 무기가 동일하면 무시.
	// 동일하지 않다면 CombatComponent에 알림
	bool bIsServer = GetOwner()->HasAuthority();
	InventoryUpdatedEvent.Broadcast();
	if (RightWeapon)
	{
		MY_LOG(LogTemp, Log, TEXT("[%s]RightWeapon changed to %s"),
			bIsServer ? TEXT("Server") : TEXT("Client"), *RightWeapon->GetName());
	}
	else
	{
		MY_LOG(LogTemp, Log, TEXT("[%s]RightWeapon is now nullptr"),
			bIsServer ? TEXT("Server") : TEXT("Client"));
	}

	if (ADDSPlayerState* PlayerState = Cast<ADDSPlayerState>(GetOwner()))
	{
		APlayerBase* PlayerBase = Cast<APlayerBase>(PlayerState->GetPawn());
			MY_LOG(LogTemp, Log, TEXT("[%s] Owner: %s"), 
        		bIsServer ? TEXT("Server") : TEXT("Client"), *PlayerBase->GetName());
		if (UPlayerCombatComponent* CombatComponent = PlayerBase->GetCombatComponent())
		{
			if (RightWeapon)
			{
				if (CombatComponent->GetRightWeaponItem() != RightWeapon)
				{
					CombatComponent->NotifyRightWeaponChanged(RightWeapon);
					MY_LOG(LogTemp, Log, TEXT("CombatComponent notified of RightWeapon change."));
				}
				else
				{
					MY_LOG(LogTemp, Log, TEXT("CombatComponent already has the correct RightWeapon."));
				}
			}else
			{
				CombatComponent->NotifyRightWeaponChanged(nullptr);
			}
		}
	}
	else
	{
		MY_ERROR_DISPLAY_NET(true, TEXT("Owner is not a valid APlayerBase."));
	}
}
