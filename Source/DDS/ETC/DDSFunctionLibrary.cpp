// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "ETC/DDSFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CustomLog.h"
#include "DDSGameplayTags.h"
#include "GenericTeamAgentInterface.h"
#include "Components/Combat/IPawnCombatInterface.h"
#include "Character/Player/DDSPlayerState.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/DDSHUD.h"
#include "UI/DDSWidgetController.h"
#include "Items/ItemInstance/ItemInstance.h"
#include "UI/WidgetController/CraftingWidgetController.h"
 

bool UDDSFunctionLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
                                                     FWidgetControllerParams& OutParams, const ADDSHUD* OutHUD)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PlayerController)
	{
		MY_ERROR_DISPLAY(TEXT("PlayerController is nullptr"));
		return false;
	}
	OutHUD = Cast<ADDSHUD>(PlayerController->GetHUD());
	if (!OutHUD)
	{
		MY_ERROR_DISPLAY(TEXT("HUD is nullptr"));
		return false;
	}
	OutParams.PlayerController = PlayerController;
	ADDSPlayerState* PlayerState = Cast<ADDSPlayerState>(PlayerController->PlayerState);
	OutParams.PlayerState = PlayerState;
	OutParams.AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	OutParams.AttributeSet = PlayerState->GetAttributeSet();
	return true;
}

UCraftingWidgetController* UDDSFunctionLibrary::GetCraftingWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ADDSHUD* OutHUD = WorldContextObject->GetWorld()->GetFirstPlayerController()->GetHUD<ADDSHUD>();
	if (MakeWidgetControllerParams(WorldContextObject, Params, OutHUD))
	{
		return OutHUD->GetCraftingWidgetController(Params);
	}
	return nullptr;
}

UDDSWidgetController* UDDSFunctionLibrary::GetWeaponCraftWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ADDSHUD* OutHUD = WorldContextObject->GetWorld()->GetFirstPlayerController()->GetHUD<ADDSHUD>();
	if (MakeWidgetControllerParams(WorldContextObject, Params, OutHUD))
	{
		return OutHUD->GetWeaponCraftWidgetController(Params);
	}
	MY_ERROR_DISPLAY(TEXT("OutHUD is nullptr"));
	return nullptr;
}

UDDSWidgetController* UDDSFunctionLibrary::GetSystemWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ADDSHUD* OutHUD = WorldContextObject->GetWorld()->GetFirstPlayerController()->GetHUD<ADDSHUD>();
	if (MakeWidgetControllerParams(WorldContextObject, Params, OutHUD))
	{
		return OutHUD->GetSystemWidgetController(Params);
	}
	MY_ERROR_DISPLAY(TEXT("OutHUD is nullptr"));
	return nullptr;
}

UDDSWidgetController* UDDSFunctionLibrary::GetStatusWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ADDSHUD* OutHUD = WorldContextObject->GetWorld()->GetFirstPlayerController()->GetHUD<ADDSHUD>();
	if (MakeWidgetControllerParams(WorldContextObject, Params, OutHUD))
	{
		return OutHUD->GetStatusWidgetController(Params);
	}
	MY_ERROR_DISPLAY(TEXT("OutHUD is nullptr"));
	return nullptr;
}

UDDSWidgetController* UDDSFunctionLibrary::GetDefualtWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ADDSHUD* OutHUD = WorldContextObject->GetWorld()->GetFirstPlayerController()->GetHUD<ADDSHUD>();
	if (MakeWidgetControllerParams(WorldContextObject, Params, OutHUD))
	{
		return OutHUD->GetDefualtWidgetController(Params);
	}
	MY_ERROR_DISPLAY(TEXT("OutHUD is nullptr"));
	return nullptr;
}

UDDSWidgetController* UDDSFunctionLibrary::GetMainMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ADDSHUD* OutHUD = WorldContextObject->GetWorld()->GetFirstPlayerController()->GetHUD<ADDSHUD>();
	if (MakeWidgetControllerParams(WorldContextObject, Params, OutHUD))
	{
		return OutHUD->GetMainMenuWidgetController(Params);
	}
	MY_ERROR_DISPLAY(TEXT("OutHUD is nullptr"));
	return nullptr;
}

UEquipWidgetController* UDDSFunctionLibrary::GetEquipWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ADDSHUD* OutHUD = WorldContextObject->GetWorld()->GetFirstPlayerController()->GetHUD<ADDSHUD>();
	if (MakeWidgetControllerParams(WorldContextObject, Params, OutHUD))
	{
		return OutHUD->GetEquipWidgetController(Params);
	}
	MY_ERROR_DISPLAY(TEXT("OutHUD is nullptr"));
	return nullptr;
}

UDDSWidgetController* UDDSFunctionLibrary::GetInventoryWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams Params;
	ADDSHUD* OutHUD = WorldContextObject->GetWorld()->GetFirstPlayerController()->GetHUD<ADDSHUD>();
	if (MakeWidgetControllerParams(WorldContextObject, Params, OutHUD))
	{
		return OutHUD->GetInventoryWidgetController(Params);
	}
	MY_ERROR_DISPLAY(TEXT("OutHUD is nullptr"));
	return nullptr;
}

UPawnCombatComponent* UDDSFunctionLibrary::NativeGetCombatComponent(AActor* InActor)
{
	if(InActor == nullptr)
	{
		return nullptr;
	}
	if (IIPawnCombatInterface* CombatInterface = Cast<IIPawnCombatInterface>(InActor))
	{
		return CombatInterface->GetCombatComponent();
	}
	return nullptr;
}

UPawnCombatComponent* UDDSFunctionLibrary::BP_GetCombatComponent(AActor* InActor, EDDSValidType& OutValid)
{
	UPawnCombatComponent* CombatComponent = NativeGetCombatComponent(InActor);
	OutValid = CombatComponent ? EDDSValidType::Valid : EDDSValidType::Invalid;
	return CombatComponent;
}

bool UDDSFunctionLibrary::NativeDoesActorHasTag(AActor* InActor, FGameplayTag InTag)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
	if (ASC)
	{
		return ASC->HasMatchingGameplayTag(InTag);
	}
	return false;
}


void UDDSFunctionLibrary::BP_DoesActorHasTag(AActor* InActor, FGameplayTag InTag, EDDSConfirmType& OutConfirm)
{
	if (NativeDoesActorHasTag(InActor, InTag))
	{
		OutConfirm = EDDSConfirmType::Yes;
	}
	else
	{
		OutConfirm = EDDSConfirmType::No;
	}
}

bool UDDSFunctionLibrary::IsValidParry(AActor* InAttacker, AActor* InDefender)
{   
	if (!InAttacker || !InDefender)
	{
		MY_ERROR_DISPLAY(TEXT("InAttacker or InDefender is nullptr"));
		return false;
	}

	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(),InDefender->GetActorForwardVector());
	const bool bIsServer = InDefender->HasAuthority();
	const bool bIsValidBlock = DotResult < -0.1f;

	MY_CLOG_DISPLAY_NET(FColor::Purple,bIsServer,TEXT("Parry Check : Attacker %s, Defender %s, DotResult : %f, bIsValidBlock : %s"),
	                   *InAttacker->GetName(),*InDefender->GetName(),DotResult,bIsValidBlock ? TEXT("true") : TEXT("false"));

	return bIsValidBlock;
}

bool UDDSFunctionLibrary::IsTargetHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	if (QueryPawn && TargetPawn)
	{
		FGenericTeamId queryTeamId = NativeGetTeamId(QueryPawn);
		FGenericTeamId targetTeamId = NativeGetTeamId(TargetPawn);
		if (queryTeamId == FGenericTeamId::NoTeam || targetTeamId == FGenericTeamId::NoTeam)
		{
			return false;
		}
		if (queryTeamId != targetTeamId)
		{
			return true;
		}
	}
	return false;
}

IGenericTeamAgentInterface* UDDSFunctionLibrary::GetTeamAgentInterface(APawn* InPawn) 
{
	if (InPawn == nullptr)
	{
		return nullptr;
	}
		
	if (IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(InPawn->Controller))
	{
		return TeamAgent;
	}

	if (IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(InPawn->GetPlayerState()))
	{
		return TeamAgent;
	}
		
	return nullptr;
}

FGenericTeamId UDDSFunctionLibrary::NativeGetTeamId(APawn* InActor)
{
	if (IGenericTeamAgentInterface * TeamAgent = GetTeamAgentInterface(InActor))
	{
		return TeamAgent->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

FGenericTeamId UDDSFunctionLibrary::BP_GetTeamId(APawn* InActor)
{
	return NativeGetTeamId(InActor);
}

EPartType UDDSFunctionLibrary::GetPartType(UItemInstance* ItemInstance)
{
	FGameplayTag TypeTag = ItemInstance->GetItemTypeTag();
	// FGameplayTagContainer GameplayTags = ItemInstance->GetItemTags();
	// if (GameplayTags.HasTagExact(DDSGameplayTags::Item_WeaponPart_Blade))
	// {
	// 	return EPartType::Blade;
	// }else if (GameplayTags.HasTagExact(DDSGameplayTags::Item_WeaponPart_Handle))
	// {
	// 	return EPartType::Handle;
	// }else if (GameplayTags.HasTagExact(DDSGameplayTags::Item_WeaponPart_Pommel))
	// {
	// 	return EPartType::Pommel;
	// }else 	if (GameplayTags.HasTag(DDSGameplayTags::Item_Weapon))
	// {
	// 	return EPartType::Full;
	// }
	return GetPartTypeByTag(TypeTag);
}

EPartType UDDSFunctionLibrary::GetPartTypeByTag(FGameplayTag TypeTag)
{
	if (TypeTag.MatchesTag(DDSGameplayTags::Item_WeaponPart_Blade))
	{
		return EPartType::Blade;
	}else if (TypeTag.MatchesTag(DDSGameplayTags::Item_WeaponPart_Grip))
	{
		return EPartType::Handle;
	}
	else 	if (TypeTag.MatchesTag(DDSGameplayTags::Item_Weapon))
	{
		return EPartType::Full;
	}
	return EPartType::None;
}
