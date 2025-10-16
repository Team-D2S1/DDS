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
	for(auto& SkillInfo : MonsterSkillClassInfos)
	{
		if(SkillInfo.MonsterSkillClass)
		{
			FGameplayAbilitySpec Spec(SkillInfo.MonsterSkillClass, 1, INDEX_NONE);
			Monster->GetAbilitySystemComponent()->GiveAbility(Spec);

			if(SkillInfo.bIsUsable)
			{
				if(UMonsterSkillBase* MonsterSkill = Cast<UMonsterSkillBase>(Spec.Ability))
				{
					MonsterSkills.Add(FCurrentSkillInfo(MonsterSkill, SkillInfo.BaseSkillWeight));
				}
			}
		}
	}
}