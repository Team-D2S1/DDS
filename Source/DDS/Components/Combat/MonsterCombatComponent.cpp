// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/MonsterCombatComponent.h"

#include "AI/Skills/MonsterSkillBase.h"
#include "Character/Monster/MonsterBase.h"
#include "ETC/CustomLog.h"


UMonsterCombatComponent::UMonsterCombatComponent()
{
	
}

void UMonsterCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for(auto Skill : MonsterSkills)
	{
		Skill->Tick(DeltaTime);
	}
}
