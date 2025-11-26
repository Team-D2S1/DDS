// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "UI/DDSUserWidget.h"

#include "Components/UI/MonsterUIComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "ETC/CustomLog.h"
#include "HUD/DDSHUD.h"
#include "Interfaces/PawnUIInterface.h"

// void UDDSUserWidget::SetWidgetController(UObject* InWidgetController)
// {
// 	WidgetController = InWidgetController;
// 	// OnWidgetConstructed();
// }

void UDDSUserWidget::SetWidgetController(UObject* InWidgetController)
{
	if (!InWidgetController)
	{
		MY_ERROR_DISPLAY(TEXT("InWidgetController is nullptr"));
		return;
	}
	WidgetController = InWidgetController;
	MY_LOG(LogTemp, Log, TEXT("WidgetController is set to %s"), *InWidgetController->GetName());
	BP_OnWidgetControllerSet();
}

void UDDSUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UPlayerUIComponent* PlayerUIComponent = PawnUIInterface->GetPlayerUIComponent())
		{
			BP_OnOwningPlayerUIComponentInitialized(PlayerUIComponent);
		}
	}
}

void UDDSUserWidget::InitMonsterCreatedWidget(AActor* OwningMonsterActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningMonsterActor))
	{
		if (UMonsterUIComponent* MonsterUIComponent = PawnUIInterface->GetMonsterUIComponent())
		{
			MY_LOG(LogTemp, Log, TEXT("InitMonsterCreatedWidget: Found MonsterUIComponent from %s"), *OwningMonsterActor->GetName());
			BP_OnOwningMonsterUIComponentInitialized(MonsterUIComponent);
		}else
		{
			MY_ERROR_DISPLAY(TEXT("Couldn't get MonsterUIComponent from %s"), *OwningMonsterActor->GetName());
		}
	}
}
