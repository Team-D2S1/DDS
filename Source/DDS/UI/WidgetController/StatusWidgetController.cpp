// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#include "UI/WidgetController/StatusWidgetController.h"
#include "GameAbilitySystem/DDSAttributeSet.h"
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameAbilitySystem/Data/AttributeInfo.h"

void UStatusWidgetController::BroadcastInitialValue()
{
	UDDSAttributeSet* AS = CastChecked<UDDSAttributeSet>(AttributeSet);

	for (auto& Pair : AS->TagToAttributeMap)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	// AttributePoints 초기값 브로드캐스트
	AttributePointsChangedDelegate.Broadcast(AS->GetAttributePoints());
}

void UStatusWidgetController::BindCallbacksToDependencies()
{
	UDDSAttributeSet* AS = CastChecked<UDDSAttributeSet>(AttributeSet);
	
	// 모든 Attribute 값 변경을 Listen
	for (auto& Pair : AS->TagToAttributeMap)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	// AttributePoints 변경 Listen
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetAttributePointsAttribute()).AddLambda(
		[this, AS](const FOnAttributeChangeData& Data)
		{
			AttributePointsChangedDelegate.Broadcast(Data.NewValue);
		}
	);
}

void UStatusWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (UDDSAbilitySystemComponent* DDSASC = GetDDSAbilitySystemComponent())
	{
		DDSASC->Server_UseAttributePointToAttribute(AttributeTag);
	}
}

void UStatusWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	if (!AttributeInfo)
	{
		return;
	}

	FDDSAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

