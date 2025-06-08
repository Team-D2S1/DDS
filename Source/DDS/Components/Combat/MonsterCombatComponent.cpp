// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/MonsterCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "AI/Skills/MonsterSkillBase.h"
#include "Character/Monster/MonsterBase.h"
#include "ETC/CustomLog.h"


UMonsterCombatComponent::UMonsterCombatComponent()
{
	
}

void UMonsterCombatComponent::BeginPlay()
{
	if(!GetOwner()->HasAuthority()) return;

	AMonsterBase* Monster = Cast<AMonsterBase>(GetOwner());
	for(auto Skill : MonsterSkills)
	{
		FGameplayAbilitySpec Spec(Skill);
		Monster->GetAbilitySystemComponent()->GiveAbility(Spec);
	}
}