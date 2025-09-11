// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "UI/HUD/DDSHUD.h"

#include "DDSGameplayTags.h"
#include "Blueprint/UserWidget.h"
#include "ETC/CustomLog.h"
#include "ETC/DDSFunctionLibrary.h"
#include "PlayerController/InGamePlayerController.h"
#include "UI/DDSUserWidget.h"
#include "UI/WidgetController/CraftingWidgetController.h"
#include "UI/WidgetController/EquipWidgetController.h"


UDDSWidgetController* ADDSHUD::GetDefualtWidgetController(const FWidgetControllerParams& params)
{
	UDDSWidgetController* WidgetController = NewObject<UDDSWidgetController>(this, UDDSWidgetController::StaticClass());
	WidgetController->SetWidgetControllerParms(params);
	WidgetController->BindCallbacksToDependencies();
    return WidgetController;
}

UUserWidget* ADDSHUD::ToggleMainMenuWidget()
{
	MY_LOG(LogTemp, Log, TEXT("ToggleMainMenuWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentMainMenuWidget) && CurrentMainMenuWidget->IsInViewport())
	{
		CloseMainMenuWidget();
		return CurrentMainMenuWidget;
	}
	else
	{
		return OpenMainMenuWidget();
	}
}

UUserWidget* ADDSHUD::OpenMainMenuWidget()
{
	MY_LOG(LogTemp, Log, TEXT("OpenMainMenuWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentMainMenuWidget) && CurrentMainMenuWidget->IsInViewport())
	{
		MY_ERROR_DISPLAY(TEXT("CurrentMainMenuWidget is already in viewport"));
		return CurrentMainMenuWidget;
	}
	
	if (!MainMenuWidgetSpec.WidgetClass)
	{
		MY_ERROR_DISPLAY(TEXT("MainMenuWidgetClass is nullptr"));
		return nullptr;
	}
	
	UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MainMenuWidgetSpec.WidgetClass);
	if (!MainMenuWidget)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to create MainMenuWidget"));
		return nullptr;
	}
	MainMenuWidget->AddToViewport();
	CurrentMainMenuWidget = MainMenuWidget;
	// CurrentMainMenuWidget->SetIsFocusable(true);
	OwningPlayerController->bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	OwningPlayerController->SetIgnoreGameInput(true);
	CurrentOpeningWidget = Cast<UDDSUserWidget>(MainMenuWidget);
	
	return MainMenuWidget;
}

void ADDSHUD::CloseMainMenuWidget()
{
	MY_LOG(LogTemp, Log, TEXT("CloseMainMenuWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return;
	}
	if (IsValid(CurrentMainMenuWidget))
	{
		CurrentMainMenuWidget->RemoveFromParent();
		CurrentMainMenuWidget = nullptr;
		CurrentOpeningWidget = nullptr;

		OwningPlayerController->bShowMouseCursor = false;
		OwningPlayerController->SetIgnoreGameInput(false);
		return;
	}
}

UDDSWidgetController* ADDSHUD::GetMainMenuWidgetController(FWidgetControllerParams& params)
{
	if (!MainMenuWidgetController)
	{
		if (MainMenuWidgetSpec.WidgetControllerClass)
		{
			MainMenuWidgetController = NewObject<UDDSWidgetController>(this, MainMenuWidgetSpec.WidgetControllerClass);
			MainMenuWidgetController->SetWidgetControllerParms(params);
			MainMenuWidgetController->BindCallbacksToDependencies();
		}
		else
		{
			MY_ERROR_DISPLAY(TEXT("MainMenuWidgetControllerClass is nullptr"));
			return nullptr;
		}
	}
	return MainMenuWidgetController;
}

UUserWidget* ADDSHUD::ToggleEquipWidget()
{
	MY_LOG(LogTemp, Log, TEXT("ToggleEquipWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentEquipWidget) && CurrentEquipWidget->IsInViewport())
	{
		CloseEquipWidget();
		return CurrentEquipWidget;
	}
	else
	{
		return OpenEquipWidget();
	}
}

UUserWidget* ADDSHUD::OpenEquipWidget()
{
	MY_LOG(LogTemp, Log, TEXT("OpenEquipWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentEquipWidget) && CurrentEquipWidget->IsInViewport())
	{
		MY_ERROR_DISPLAY(TEXT("CurrentEquipWidget is already in viewport"));
		return CurrentEquipWidget;
	}
	
	if (!EquipWidgetSpec.WidgetClass)
	{
		MY_ERROR_DISPLAY(TEXT("EquipWidgetClass is nullptr"));
		return nullptr;
	}
	
	UUserWidget* EquipWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), EquipWidgetSpec.WidgetClass);
	if (!EquipWidget)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to create EquipWidget"));
		return nullptr;
	}
	EquipWidget->AddToViewport();
	CurrentEquipWidget = EquipWidget;
	// CurrentEquipWidget->SetIsFocusable(true);
	OwningPlayerController->bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(EquipWidget->TakeWidget());
	OwningPlayerController->SetIgnoreGameInput(true);
	
	CurrentOpeningWidget = Cast<UDDSUserWidget>(EquipWidget);
	
	return EquipWidget;
}

void ADDSHUD::CloseEquipWidget()
{
	MY_LOG(LogTemp, Log, TEXT("CloseEquipWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return;
	}
	if (IsValid(CurrentEquipWidget))
	{
		CurrentEquipWidget->RemoveFromParent();
		CurrentEquipWidget = nullptr;
		CurrentOpeningWidget = nullptr;

		OwningPlayerController->bShowMouseCursor = false;
		OwningPlayerController->SetIgnoreGameInput(false);
		return;
	}
}

UEquipWidgetController* ADDSHUD::GetEquipWidgetController(FWidgetControllerParams& params)
{
	if (!EquipWidgetController)
	{
		if (EquipWidgetSpec.WidgetControllerClass)
		{
			EquipWidgetController = NewObject<UEquipWidgetController>(this, EquipWidgetSpec.WidgetControllerClass);
			EquipWidgetController->SetWidgetControllerParms(params);
			EquipWidgetController->BindCallbacksToDependencies();
		}
		else
		{
			MY_ERROR_DISPLAY(TEXT("EquipWidgetControllerClass is nullptr"));
			return nullptr;
		}
	}
	return EquipWidgetController;
}

UUserWidget* ADDSHUD::ToggleInventoryWidget()
{
	MY_LOG(LogTemp, Log, TEXT("ToggleInventoryWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentInventoryWidget) && CurrentInventoryWidget->IsInViewport())
	{
		CloseInventoryWidget();
		return CurrentInventoryWidget;
	}
	else
	{
		return OpenInventoryWidget();
	}
}

UUserWidget* ADDSHUD::OpenInventoryWidget()
{
	MY_LOG(LogTemp, Log, TEXT("OpenInventoryWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentInventoryWidget) && CurrentInventoryWidget->IsInViewport())
	{
		MY_ERROR_DISPLAY(TEXT("CurrentInventoryWidget is already in viewport"));
		return CurrentInventoryWidget;
	}
	
	if (!InventoryWidgetSpec.WidgetClass)
	{
		MY_ERROR_DISPLAY(TEXT("InventoryWidgetClass is nullptr"));
		return nullptr;
	}
	
	UUserWidget* InventoryWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), InventoryWidgetSpec.WidgetClass);
	if (!InventoryWidget)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to create InventoryWidget"));
		return nullptr;
	}
	InventoryWidget->AddToViewport();
	CurrentInventoryWidget = InventoryWidget;
	CurrentInventoryWidget->SetIsFocusable(true);
	// OwningPlayerController->bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
	OwningPlayerController->SetIgnoreGameInput(true);
	
	CurrentOpeningWidget = Cast<UDDSUserWidget>(InventoryWidget);
	
	return InventoryWidget;
}

void ADDSHUD::CloseInventoryWidget()
{
	MY_LOG(LogTemp, Log, TEXT("CloseInventoryWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return;
	}
	if (IsValid(CurrentInventoryWidget))
	{
		CurrentInventoryWidget->RemoveFromParent();
		CurrentInventoryWidget = nullptr;
		CurrentOpeningWidget = nullptr;

		OwningPlayerController->bShowMouseCursor = false;
		OwningPlayerController->SetIgnoreGameInput(false);
		return;
	}
}

UDDSWidgetController* ADDSHUD::GetInventoryWidgetController(FWidgetControllerParams& params)
{
	if (!InventoryWidgetController)
	{
		if (InventoryWidgetSpec.WidgetControllerClass)
		{
			InventoryWidgetController = NewObject<UDDSWidgetController>(this, InventoryWidgetSpec.WidgetControllerClass);
			InventoryWidgetController->SetWidgetControllerParms(params);
			InventoryWidgetController->BindCallbacksToDependencies();
		}
		else
		{
			MY_ERROR_DISPLAY(TEXT("InventoryWidgetControllerClass is nullptr"));
			return nullptr;
		}
	}
	return InventoryWidgetController;
}
UUserWidget* ADDSHUD::ToggleWeaponCraftWidget()
{
	MY_LOG(LogTemp, Log, TEXT("ToggleWeaponCraftWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentWeaponCraftWidget) && CurrentWeaponCraftWidget->IsInViewport())
	{
		CloseWeaponCraftWidget();
		return CurrentWeaponCraftWidget;
	}
	else
	{
		return OpenWeaponCraftWidget();
	}
}

UUserWidget* ADDSHUD::OpenWeaponCraftWidget()
{
	MY_LOG(LogTemp, Log, TEXT("OpenWeaponCraftWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentWeaponCraftWidget) && CurrentWeaponCraftWidget->IsInViewport())
	{
		MY_ERROR_DISPLAY(TEXT("CurrentWeaponCraftWidget is already in viewport"));
		return CurrentWeaponCraftWidget;
	}
	
	if (!WeaponCraftWidgetSpec.WidgetClass)
	{
		MY_ERROR_DISPLAY(TEXT("WeaponCraftWidgetClass is nullptr"));
		return nullptr;
	}
	
	UUserWidget* WeaponCraftWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), WeaponCraftWidgetSpec.WidgetClass);
	if (!WeaponCraftWidget)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to create WeaponCraftWidget"));
		return nullptr;
	}
	WeaponCraftWidget->AddToViewport();
	CurrentWeaponCraftWidget = WeaponCraftWidget;
	// CurrentWeaponCraftWidget->SetIsFocusable(true);
	OwningPlayerController->bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(WeaponCraftWidget->TakeWidget());
	OwningPlayerController->SetIgnoreGameInput(true);
	
	CurrentOpeningWidget = Cast<UDDSUserWidget>(WeaponCraftWidget);
	
	return WeaponCraftWidget;
}

void ADDSHUD::CloseWeaponCraftWidget()
{
	MY_LOG(LogTemp, Log, TEXT("CloseWeaponCraftWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return;
	}
	if (IsValid(CurrentWeaponCraftWidget))
	{
		CurrentWeaponCraftWidget->RemoveFromParent();
		CurrentWeaponCraftWidget = nullptr;
		CurrentOpeningWidget = nullptr;
	
		OwningPlayerController->bShowMouseCursor = false;
		OwningPlayerController->SetIgnoreGameInput(false);
		return;
	}
}

UDDSWidgetController* ADDSHUD::GetWeaponCraftWidgetController(FWidgetControllerParams& params)
{
	if (!WeaponCraftWidgetController)
	{
		if (WeaponCraftWidgetSpec.WidgetControllerClass)
		{
			WeaponCraftWidgetController = NewObject<UDDSWidgetController>(this, WeaponCraftWidgetSpec.WidgetControllerClass);
			WeaponCraftWidgetController->SetWidgetControllerParms(params);
			WeaponCraftWidgetController->BindCallbacksToDependencies();
		}
		else
		{
			MY_ERROR_DISPLAY(TEXT("WeaponCraftWidgetControllerClass is nullptr"));
			return nullptr;
		}
	}
	return WeaponCraftWidgetController;
}

UUserWidget* ADDSHUD::ToggleCraftingWidget()
{
	MY_LOG(LogTemp, Log, TEXT("ToggleCraftingWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentCraftingWidget) && CurrentCraftingWidget->IsInViewport())
	{
		CloseCraftingWidget();
		return CurrentCraftingWidget;
	}
	else
	{
		return OpenCraftingWidget();
	}
	
}
UUserWidget* ADDSHUD::OpenCraftingWidget()
{
	MY_LOG(LogTemp, Log, TEXT("OpenCraftingWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentCraftingWidget) && CurrentCraftingWidget->IsInViewport())
	{
		MY_ERROR_DISPLAY(TEXT("CurrentCraftingWidget is already in viewport"));
		return CurrentCraftingWidget;
	}
	
	if (!CraftingWidgetSpec.WidgetClass)
	{
		MY_ERROR_DISPLAY(TEXT("CraftingWidgetClass is nullptr"));
		return nullptr;
	}
	
	UUserWidget* CraftingWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), CraftingWidgetSpec.WidgetClass);
	if (!CraftingWidget)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to create CraftingWidget"));
		return nullptr;
	}
	CraftingWidget->AddToViewport();
	CurrentCraftingWidget = CraftingWidget;
	// CurrentCraftingWidget->SetIsFocusable(true);
	OwningPlayerController->bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(CraftingWidget->TakeWidget());
	OwningPlayerController->SetIgnoreGameInput(true);
	// CraftingWidget->SetPositionInViewport(FVector2D(0, 0), true);

	CurrentOpeningWidget = Cast<UDDSUserWidget>(CraftingWidget);
	
	return CraftingWidget;
}
void ADDSHUD::CloseCraftingWidget()
{
	MY_LOG(LogTemp, Log, TEXT("CloseCraftingWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return;
	}
	if (IsValid(CurrentCraftingWidget))
    {
    	CurrentCraftingWidget->RemoveFromParent();
    	CurrentCraftingWidget = nullptr;
		CurrentOpeningWidget = nullptr;

    	// OwningPlayerController->SetInputMode(FInputModeGameAndUI());
    	OwningPlayerController->bShowMouseCursor = false;
		OwningPlayerController->SetIgnoreGameInput(false);
    	return;
    }
}

UCraftingWidgetController* ADDSHUD::GetCraftingWidgetController(FWidgetControllerParams& params)
{
	if (!CraftingWidgetController)
	{
		if (CraftingWidgetSpec.WidgetControllerClass)
		{
			CraftingWidgetController = NewObject<UCraftingWidgetController>(this, CraftingWidgetSpec.WidgetControllerClass);
			CraftingWidgetController->SetWidgetControllerParms(params);
			CraftingWidgetController->BindCallbacksToDependencies();
		}
		else
		{
			MY_ERROR_DISPLAY(TEXT("CraftingWidgetControllerClass is nullptr"));
			return nullptr;
		}
	}
	return CraftingWidgetController;
}

UUserWidget* ADDSHUD::ToggleSatatusWidget()
{
	MY_LOG(LogTemp, Log, TEXT("ToggleStatusWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentOpeningWidget) && CurrentOpeningWidget->IsInViewport())
	{
		CloseStatusWidget();
		return CurrentOpeningWidget;
	}
	else
	{
		return OpenStatusWidget();
	}
}

UUserWidget* ADDSHUD::OpenStatusWidget()
{
	MY_LOG(LogTemp, Log, TEXT("OpenStatusWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentOpeningWidget) && CurrentOpeningWidget->IsInViewport())
	{
		MY_ERROR_DISPLAY(TEXT("CurrentOpeningWidget is already in viewport"));
		return CurrentOpeningWidget;
	}
	
	if (!StatusWidgetSpec.WidgetClass)
	{
		MY_ERROR_DISPLAY(TEXT("StatusWidgetClass is nullptr"));
		return nullptr;
	}
	
	UUserWidget* StatusWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), StatusWidgetSpec.WidgetClass);
	if (!StatusWidget)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to create StatusWidget"));
		return nullptr;
	}
	StatusWidget->AddToViewport();
	CurrentOpeningWidget = Cast<UDDSUserWidget>(StatusWidget);
	
	// StatusWidget->SetIsFocusable(true);
	OwningPlayerController->bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(StatusWidget->TakeWidget());
	OwningPlayerController->SetIgnoreGameInput(true);
	
	return StatusWidget;
}

void ADDSHUD::CloseStatusWidget()
{
	MY_LOG(LogTemp, Log, TEXT("CloseStatusWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return;
	}
	if (IsValid(CurrentOpeningWidget))
	{
		CurrentOpeningWidget->RemoveFromParent();
		CurrentOpeningWidget = nullptr;

		OwningPlayerController->bShowMouseCursor = false;
		OwningPlayerController->SetIgnoreGameInput(false);
		return;
	}
}

UDDSWidgetController* ADDSHUD::GetStatusWidgetController(FWidgetControllerParams& params)
{
	if (!StatusWidgetController)
	{
		if (StatusWidgetSpec.WidgetControllerClass)
		{
			StatusWidgetController = NewObject<UDDSWidgetController>(this, StatusWidgetSpec.WidgetControllerClass);
			StatusWidgetController->SetWidgetControllerParms(params);
			StatusWidgetController->BindCallbacksToDependencies();
		}
		else
		{
			MY_ERROR_DISPLAY(TEXT("StatusWidgetControllerClass is nullptr"));
			return nullptr;
		}
	}
	return StatusWidgetController;
}

UUserWidget* ADDSHUD::ToggleSystemWidget()
{
	MY_LOG(LogTemp, Log, TEXT("ToggleSystemWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentOpeningWidget) && CurrentOpeningWidget->IsInViewport())
	{
		CloseSystemWidget();
		return CurrentOpeningWidget;
	}
	else
	{
		return OpenSystemWidget();
	}
}

UUserWidget* ADDSHUD::OpenSystemWidget()
{
	MY_LOG(LogTemp, Log, TEXT("OpenSystemWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentOpeningWidget) && CurrentOpeningWidget->IsInViewport())
	{
		MY_ERROR_DISPLAY(TEXT("CurrentOpeningWidget is already in viewport"));
		return CurrentOpeningWidget;
	}
	
	if (!SystemWidgetSpec.WidgetClass)
	{
		MY_ERROR_DISPLAY(TEXT("SystemWidgetClass is nullptr"));
		return nullptr;
	}
	
	UUserWidget* SystemWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), SystemWidgetSpec.WidgetClass);
	if (!SystemWidget)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to create SystemWidget"));
		return nullptr;
	}
	SystemWidget->AddToViewport();
	
	// SystemWidget->SetIsFocusable(true);
	OwningPlayerController->bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(SystemWidget->TakeWidget());
	OwningPlayerController->SetIgnoreGameInput(true);
	
	CurrentOpeningWidget = Cast<UDDSUserWidget>(SystemWidget);
	
	return SystemWidget;
}

void ADDSHUD::CloseSystemWidget()
{
	MY_LOG(LogTemp, Log, TEXT("CloseSystemWidget()"));
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return;
	}
	if (IsValid(CurrentOpeningWidget))
	{
		CurrentOpeningWidget->RemoveFromParent();
		CurrentOpeningWidget = nullptr;

		OwningPlayerController->bShowMouseCursor = false;
		OwningPlayerController->SetIgnoreGameInput(false);
		return;
	}
}

UDDSWidgetController* ADDSHUD::GetSystemWidgetController(FWidgetControllerParams& params)
{
	if (!SystemWidgetController)
	{
		if (SystemWidgetSpec.WidgetControllerClass)
		{
			SystemWidgetController = NewObject<UDDSWidgetController>(this, SystemWidgetSpec.WidgetControllerClass);
			SystemWidgetController->SetWidgetControllerParms(params);
			SystemWidgetController->BindCallbacksToDependencies();
		}
		else
		{
			MY_ERROR_DISPLAY(TEXT("SystemWidgetControllerClass is nullptr"));
			return nullptr;
		}
	}
	return SystemWidgetController;
}


bool ADDSHUD::HandleInputAction(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		MY_ERROR_DISPLAY(TEXT("InputTag %s is invalid"),*InputTag.ToString());
		return false;
	}
	MY_LOG(LogTemp, Log, TEXT("Handle Input Action %s"), *InputTag.ToString());
	
	if (InputTag.MatchesTag(DDSGameplayTags::InputTag_UI_Escape))
	{
		if (CurrentOpeningWidget == nullptr)
		{
			ToggleMainMenuWidget();
			return true;
		}else
		{
			CurrentOpeningWidget->BP_HandleInputAction(InputTag);
			return true;
		}
	}

	if (InputTag.MatchesTagExact(DDSGameplayTags::InputTag_UI_Escape))
	{
		if (CurrentOpeningWidget)
		{
			CurrentOpeningWidget->BP_HandleInputAction(InputTag);
			return true;
		}
		else
		{
			MY_ERROR_DISPLAY(TEXT("CurrentOpeningWidget is nullptr"));
			return false;
		}
	}
	// if (InputTag.MatchesTagExact(DDSGameplayTags::InputTag_UI_Crafting_RemoveItem))
	// {
	// 	// R키를 누르면 현재 아이템 삭제
	// 	
	// 	/**TODO : UCraftingWidgetController로 위임 가능
	//     * MatchesTagExact가 아닌 MatchesTag나 Any를 이용
	// 	* 하위 컨트롤러의 HandleInputAction 호출.
	// 	*/
	// 	if (CurrentCraftingWidget)
	// 	{
	// 		if (CraftingWidgetController)
	// 		{
	// 			CraftingWidgetController->OnRemoveButtonClicked();
	// 			return true;
	// 		}
	// 	}
	// }
	// else
	// {
	// 	MY_ERROR_DISPLAY(TEXT("InputTag does not match"));
	// 	return false;
	// }
	MY_ERROR_DISPLAY(TEXT("InputTag is not handled"));
	return false;
}

UUserWidget* ADDSHUD::OpenWidget(const FWidgetSpecification& WidgetSpec)
{
	UUserWidget* widget = CreateWidget<UUserWidget>(GetOwningPlayerController(), WidgetSpec.WidgetClass);
	if (!widget)
	{
		MY_ERROR_DISPLAY(TEXT("Failed to create CraftingWidget"));
		return nullptr;
	}
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	widget->AddToViewport();
	OwningPlayerController->bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(widget->TakeWidget());
	OwningPlayerController->SetIgnoreGameInput(true);
	// CraftingWidget->SetPositionInViewport(FVector2D(0, 0), true);
	CurrentOpeningWidget = Cast<UDDSUserWidget>(widget);
	if (WidgetSpec.WidgetTag.IsValid())
	{
		WidgetHistory.Add(WidgetSpec.WidgetTag);
	}
	
	return widget;
}

UUserWidget* ADDSHUD::CloseWidget(const FWidgetSpecification& WidgetSpec)
{
	AInGamePlayerController* OwningPlayerController = Cast<AInGamePlayerController>(GetOwningPlayerController());
	if (OwningPlayerController == nullptr)
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is nullptr"));
		return nullptr;
	}
	if (!OwningPlayerController->IsLocalController())
	{
		MY_ERROR_DISPLAY(TEXT("GetOwningPlayerController() is not local controller"));
		return nullptr;
	}
	if (IsValid(CurrentOpeningWidget) && CurrentOpeningWidget->IsInViewport())
	{
		if (WidgetSpec.WidgetTag.IsValid() && WidgetHistory.Contains(WidgetSpec.WidgetTag))
		{
			// 가장 뒤에 있는거 삭제
			for (int32 i = WidgetHistory.Num() - 1; i >= 0; --i)
			{
				if (WidgetHistory[i] == WidgetSpec.WidgetTag)
				{
					WidgetHistory.RemoveAt(i);
					break;
				}
			}
		}
		CurrentOpeningWidget->RemoveFromParent();
		CurrentOpeningWidget = nullptr;

		OwningPlayerController->bShowMouseCursor = false;
		OwningPlayerController->SetIgnoreGameInput(false);
		return nullptr;
	}
	return nullptr;
}
