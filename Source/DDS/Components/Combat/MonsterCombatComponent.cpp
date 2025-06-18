// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/MonsterCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "AI/Skills/MonsterSkillBase.h"
#include "Character/Monster/MonsterBase.h"



UMonsterCombatComponent::UMonsterCombatComponent()
{
	
}

void UMonsterCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(!GetOwner()->HasAuthority()) return;

	AMonsterBase* Monster = Cast<AMonsterBase>(GetOwner());
	for(auto SkillClass : MonsterSkillClass)
	{
		if(SkillClass)
		{
			FGameplayAbilitySpec Spec(SkillClass, 1, INDEX_NONE);
			Monster->GetAbilitySystemComponent()->GiveAbility(Spec);

			if(UMonsterSkillBase* MonsterSkill = Cast<UMonsterSkillBase>(Spec.Ability))
			{
				MonsterSkills.Add(MonsterSkill);
			}
		}
	}
}