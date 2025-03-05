// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DataAsset/StartUpData/DataAsset_StartUpDataBase.h"

#include "ETC/CustomLog.h"
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameAbilitySystem/Abilities/DDSGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UDDSAbilitySystemComponent* InASCToGive,
                                                              int32 ApplyLevel)
{
	if (!InASCToGive)
	{
		MY_LOG(LogTemp,Type::Log,"InDdsASCToGive is nullptr");
	}

	GrantAbilities(ActivateOnGivenAbilities,InASCToGive,ApplyLevel);
	GrantAbilities(ReactiveAbilities,InASCToGive,ApplyLevel);
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UDDSGameplayAbility>> InAbilitiesToGive,
	UDDSAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}
	InASCToGive->AddCharacterAbilities(InAbilitiesToGive);
}
