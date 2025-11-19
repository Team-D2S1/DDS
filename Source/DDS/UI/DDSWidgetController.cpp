// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "UI/DDSWidgetController.h"

#include "Character/Player/DDSPlayerState.h"
#include "Character/Player/PlayerBase.h"
#include "Components/Inventory/InventoryComponent.h"
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameAbilitySystem/DDSAttributeSet.h"
#include "PlayerController/InGamePlayerController.h"
#include "UI/HUD/DDSHUD.h"


void UDDSWidgetController::SetWidgetControllerParms(FWidgetControllerParams InParams)
{
	PlayerController = InParams.PlayerController;
	PlayerState = InParams.PlayerState;
	AbilitySystemComponent = InParams.AbilitySystemComponent;
	AttributeSet = InParams.AttributeSet;

	if (PlayerController)
	{
		InGamePlayerController = Cast<AInGamePlayerController>(PlayerController);
	}
	if (PlayerState)
	{
		DDSPlayerState = Cast<ADDSPlayerState>(PlayerState);
	}
	if (AbilitySystemComponent)
	{
		DDSAbilitySystemComponent = Cast<UDDSAbilitySystemComponent>(AbilitySystemComponent);
	}
	if (AttributeSet)
	{
		DDSAttributeSet = Cast<UDDSAttributeSet>(AttributeSet);
	}
}

void UDDSWidgetController::BroadcastInitialValue()
{
}

void UDDSWidgetController::BindCallbacksToDependencies()
{
}

UDDSAttributeSet* UDDSWidgetController::GetDDSAttributeSet()
{
	if (!DDSAttributeSet)
	{
		DDSAttributeSet = Cast<UDDSAttributeSet>(AttributeSet);
	}
	return DDSAttributeSet;
}

UDDSAbilitySystemComponent* UDDSWidgetController::GetDDSAbilitySystemComponent()
{
	if (!DDSAbilitySystemComponent)
	{
		DDSAbilitySystemComponent = Cast<UDDSAbilitySystemComponent>(AbilitySystemComponent);
	}
	return DDSAbilitySystemComponent;
}

AInGamePlayerController* UDDSWidgetController::GetDDSPlayerController()
{
	if (!PlayerController)
	{
		InGamePlayerController = Cast<AInGamePlayerController>(PlayerController);
	}
	return InGamePlayerController;
}

ADDSPlayerState* UDDSWidgetController::GetDDSPlayerState()
{
	if (!PlayerState)
	{
		DDSPlayerState = Cast<ADDSPlayerState>(PlayerState);
	}
	return DDSPlayerState;
}

ADDSHUD* UDDSWidgetController::GetDDSHUD()
{
	if (!ChachedDDSHUD.IsValid())
	{
		if (AInGamePlayerController* InGamePC = GetDDSPlayerController())
		{
			ChachedDDSHUD = InGamePC->GetHUD<ADDSHUD>();
		}
	}
	return ChachedDDSHUD.Get();
}

UPlayerCombatComponent* UDDSWidgetController::GetPlayerCombatComponent() 
{
	return GetDDSPlayerController()->GetPlayerBase()->GetCombatComponent();
}

UInventoryComponent* UDDSWidgetController::GetInventoryComponent() const
{
	return DDSPlayerState->GetInventoryComponent();
}
