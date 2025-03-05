// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DataAsset/StartUpData/DataAsset_PlayerStartUpData.h"

#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameAbilitySystem/Abilities/DDSGameplayAbility.h"

bool FDDSPlayerAbilitySet::IsValid() const
{
   	return InputTag.IsValid() && AbilityToGrant;
}

void UDataAsset_PlayerStartUpData::GiveToAbilitySystemComponent(UDDSAbilitySystemComponent* InASCToGive,
	int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	for (const FDDSPlayerAbilitySet& AbilitySet : PlayerStartUpAbilitiesSet)
	{
		if (!AbilitySet.IsValid())
		{
			continue;
		}
		FGameplayAbilitySpec spec(AbilitySet.AbilityToGrant);
		spec.SourceObject = InASCToGive->GetAvatarActor();
		spec.Level  = ApplyLevel;
		spec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
		InASCToGive->GiveAbility(spec);
	}
}
