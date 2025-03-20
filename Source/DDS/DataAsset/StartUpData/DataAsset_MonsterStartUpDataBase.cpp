// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DataAsset/StartUpData/DataAsset_MonsterStartUpDataBase.h"

#include "GameplayAbilitySpec.h"
#include "ETC/CustomLog.h"
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameAbilitySystem/Abilities/DDSMonsterGameplayAbility.h"


void UDataAsset_MonsterStartUpDataBase::GiveToAbilitySystemComponent(UDDSAbilitySystemComponent* InASCToGive,
                                                                     int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);
	
	if (!MonsterCombatAbilities.IsEmpty())
	{
		for (const TSubclassOf<UDDSMonsterGameplayAbility>& AbilityClass : MonsterCombatAbilities)
		{
			if (!AbilityClass)
				continue;
			FGameplayAbilitySpec spec(AbilityClass);
			spec.SourceObject = InASCToGive->GetAvatarActor();
			spec.Level = ApplyLevel;
			InASCToGive->GiveAbility(spec);
			DEBUG_CLOG_DISPLAY(FColor::Green, "Monster StartUpData Loaded : %s", *AbilityClass->GetName());
		}
	}
}
