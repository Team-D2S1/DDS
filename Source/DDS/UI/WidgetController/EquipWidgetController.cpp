// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "UI/WidgetController/EquipWidgetController.h"

#include "Character/Player/DDSPlayerState.h"
#include "ETC/CustomLog.h"


void UEquipWidgetController::BroadcastInitialValue()
{
	Super::BroadcastInitialValue();
}

void UEquipWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	if (UInventoryComponent* InvComp = GetInventoryComponent())
	{
		MY_LOG(LogTemp, Log, TEXT("Binding InventoryComponent callbacks"));
		InvComp->InventoryUpdatedEvent.AddDynamic(this, &UEquipWidgetController::OnInventoryUpdated);
	}
}



void UEquipWidgetController::OnInventoryUpdated()
{
	MY_LOG(LogTemp, Log, TEXT("OnInventoryUpdated called"));
    BP_UpdateUI.Broadcast();
}
