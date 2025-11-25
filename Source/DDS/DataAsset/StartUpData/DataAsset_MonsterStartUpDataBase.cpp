// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DataAsset/StartUpData/DataAsset_MonsterStartUpDataBase.h"

#include "GameplayAbilitySpec.h"
#include "ETC/CustomLog.h"
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameAbilitySystem/DDSAttributeSet.h"
#include "GameAbilitySystem/Abilities/DDSMonsterGameplayAbility.h"


void UDataAsset_MonsterStartUpDataBase::GiveToAbilitySystemComponent(UDDSAbilitySystemComponent* InASCToGive,
                                                                     int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);
	// MY_LOG( LogTemp, Type::Log, "Monster StartUpData Loaded : %s", *GetName());
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
			MY_CLOG_DISPLAY(FColor::Green, "Monster StartUpData Loaded : %s", *AbilityClass->GetName());
		}
	}

	const UDDSAttributeSet* CAttributeSet = InASCToGive->GetSet<UDDSAttributeSet>();
	UDDSAttributeSet* AttributeSet = const_cast<UDDSAttributeSet*>(CAttributeSet);
	if(AttributeSet)
	{
		AttributeSet->SetHealthMax(DefaultHealth);
		AttributeSet->SetHealth(DefaultHealth);
		AttributeSet->SetAttackPower(DefaultAttackPower);
		AttributeSet->SetPhysicalDefense(DefaultDefense);
		AttributeSet->SetPhysicalResist(DefaultResistance);
		AttributeSet->SetSoul(DefaultSoulDrop);
		AttributeSet->SetEnergy(DefaultExperienceDrop);
		AttributeSet->SetRequireEnergy(DefaultExperienceDrop * 2.f);
	}
	
}
