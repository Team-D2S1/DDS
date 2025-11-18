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
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


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
		// RepObj(RightWeapon->GetPommelItemInstance());
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
                                                                 int32 BladeItemID, int32 GripItemID)
{
	if (GetOwner()->HasAuthority())
	{
		MY_LOG(LogTemp, Log, TEXT("Server_AddCraftedWeapon called. BladeItemID: %d, GripItemID: %d"), BladeItemID, GripItemID);
		UItemInstance* WeaponItemInstance = NewObject<UItemInstance>();
		if (WeaponItemInstance->Init(WeaponItemClass))
		{
			UItemInstance* BladeItemInstance = GetItemByID(BladeItemID);
			UItemInstance* GripItemInstance = GetItemByID(GripItemID);
			if (!(BladeItemInstance && GripItemInstance))
			{
				MY_ERROR_DISPLAY_NET(true, TEXT("One or more item instances are null. Blade: %d, Grip: %d"),
					BladeItemID, GripItemID);
				return;
			}
			WeaponItemInstance->SetBladeItemInstance(BladeItemInstance);
			WeaponItemInstance->SetGripItemInstance(GripItemInstance);
			AddItemInstance(WeaponItemInstance);
			
			RemoveItem(BladeItemID);
			RemoveItem(GripItemID);
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
		// 기존 무기 이펙트 제거
		RemoveItemEffect(0); // 0은 무기 슬롯
		RightWeapon = nullptr;
		if (bIsServer)
		{
			MY_LOG(LogTemp, Log, TEXT("RightWeapon is set to nullptr on server."));
			OnRep_RightWeapon();
		}
	}
	else
	{
		// 기존 무기 이펙트 제거
		RemoveItemEffect(0); // 0은 무기 슬롯
		
		RightWeapon = GetItemByID(ItemID);
		if (bIsServer)
		{
			// 새 무기 이펙트 적용
			if (RightWeapon)
			{
				ApplyItemEffect(RightWeapon, 0); // 0은 무기 슬롯
			}
			OnRep_RightWeapon();
		}
	}
}

void UInventoryComponent::Server_EquipArmor_Implementation(int32 ItemID, int32 ArmorIndex)
{
	UItemInstance*& TargetArmor = (ArmorIndex == 0) ? Armor01 :
									(ArmorIndex == 1) ? Armor02 :
									(ArmorIndex == 2) ? Armor03 :
									 Armor04;

	bool bIsServer = GetOwner()->HasAuthority();
	MY_LOG(LogTemp, Log, TEXT("[%s] Server_EquipArmor called with ItemID: %d, ArmorIndex: %d"),
		bIsServer ? TEXT("Server") : TEXT("Client"), ItemID, ArmorIndex);
	if (ItemID == 0)
	{
		// 해제만 진행
		// 기존 방어구 이펙트 제거 (1-4는 방어구 슬롯)
		RemoveItemEffect(ArmorIndex + 1);
		MY_LOG_DISPLAY_NET(bIsServer, TEXT("ArmorIndex %d is set to nullptr"), ArmorIndex);
		TargetArmor = nullptr;
		
	}
	else
	{
		// 기존 방어구 이펙트 제거
		RemoveItemEffect(ArmorIndex + 1);
		
		MY_LOG_DISPLAY_NET(bIsServer, TEXT("Equipping ArmorIndex %d with ItemID %d"), ArmorIndex, ItemID);
		TargetArmor = GetItemByID(ItemID);
		
		// 새 방어구 이펙트 적용
		if (bIsServer && TargetArmor)
		{
			ApplyItemEffect(TargetArmor, ArmorIndex + 1); // 1-4는 방어구 슬롯
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
		
		// 이벤트 호출 (서버와 클라이언트 모두)
		OnItemEquippedEvent.Broadcast(RightWeapon, 0); // 0은 무기 슬롯
	}
	else
	{
		MY_LOG(LogTemp, Log, TEXT("[%s]RightWeapon is now nullptr"),
			bIsServer ? TEXT("Server") : TEXT("Client"));
		
		// 이벤트 호출 (서버와 클라이언트 모두)
		OnItemUnequippedEvent.Broadcast(0); // 0은 무기 슬롯
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

void UInventoryComponent::OnRep_Armor01()
{
	InventoryUpdatedEvent.Broadcast();
	
	// 클라이언트 이벤트 호출 (서버는 함수에서 직접 호출)
	if (Armor01)
	{
		OnItemEquippedEvent.Broadcast(Armor01, 1); // 1은 Armor01 슬롯
	}
	else
	{
		OnItemUnequippedEvent.Broadcast(1); // 1은 Armor01 슬롯
	}
}

void UInventoryComponent::OnRep_Armor02()
{
	InventoryUpdatedEvent.Broadcast();
	
	// 클라이언트 이벤트 호출 (서버는 함수에서 직접 호출)
	if (Armor02)
	{
		OnItemEquippedEvent.Broadcast(Armor02, 2); // 2는 Armor02 슬롯
	}
	else
	{
		OnItemUnequippedEvent.Broadcast(2); // 2는 Armor02 슬롯
	}
}

void UInventoryComponent::OnRep_Armor03()
{
	InventoryUpdatedEvent.Broadcast();
	
	// 클라이언트 이벤트 호출 (서버는 함수에서 직접 호출)
	if (Armor03)
	{
		OnItemEquippedEvent.Broadcast(Armor03, 3); // 3은 Armor03 슬롯
	}
	else
	{
		OnItemUnequippedEvent.Broadcast(3); // 3은 Armor03 슬롯
	}
}

void UInventoryComponent::OnRep_Armor04()
{
	InventoryUpdatedEvent.Broadcast();
	
	// 클라이언트 이벤트 호출 (서버는 함수에서 직접 호출)
	if (Armor04)
	{
		OnItemEquippedEvent.Broadcast(Armor04, 4); // 4는 Armor04 슬롯
	}
	else
	{
		OnItemUnequippedEvent.Broadcast(4); // 4는 Armor04 슬롯
	}
}

void UInventoryComponent::ApplyItemEffect(UItemInstance* ItemInstance, int32 SlotIndex)
{
	if (!ItemInstance)
	{
		MY_ERROR_DISPLAY(TEXT("ItemInstance is nullptr"));
		return;
	}

	// AbilitySystemComponent 가져오기
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC)
	{
		MY_ERROR_DISPLAY(TEXT("AbilitySystemComponent not found"));
		return;
	}

	// 아이템의 이펙트 클래스 가져오기
	TSubclassOf<UGameplayEffect> EffectClass = ItemInstance->GetItemEffectClass();
	if (!EffectClass)
	{
		MY_LOG(LogTemp, Log, TEXT("No effect class found for item: %s"), *ItemInstance->GetItemName());
		return;
	}

	// 이펙트 적용
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(ItemInstance);
	
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);
	if (EffectSpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		if (ActiveEffectHandle.IsValid())
		{
			// 슬롯별로 활성 이펙트 저장 (나중에 제거할 때 사용)
			ActiveItemEffects.Add(SlotIndex, ActiveEffectHandle);
			MY_LOG(LogTemp, Log, TEXT("Applied effect for item: %s in slot: %d"), *ItemInstance->GetItemName(), SlotIndex);
		}
		else
		{
			MY_ERROR_DISPLAY(TEXT("Failed to apply gameplay effect"));
		}
	}
	else
	{
		MY_ERROR_DISPLAY(TEXT("Failed to create effect spec handle"));
	}
}

void UInventoryComponent::RemoveItemEffect(int32 SlotIndex)
{
	// AbilitySystemComponent 가져오기
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC)
	{
		MY_ERROR_DISPLAY(TEXT("AbilitySystemComponent not found"));
		return;
	}

	// 해당 슬롯의 활성 이펙트 찾기
	if (FActiveGameplayEffectHandle* EffectHandle = ActiveItemEffects.Find(SlotIndex))
	{
		if (EffectHandle->IsValid())
		{
			ASC->RemoveActiveGameplayEffect(*EffectHandle);
			MY_LOG(LogTemp, Log, TEXT("Removed effect for slot: %d"), SlotIndex);
		}
		ActiveItemEffects.Remove(SlotIndex);
	}
	else
	{
		MY_LOG(LogTemp, Log, TEXT("No active effect found for slot: %d"), SlotIndex);
	}
}
