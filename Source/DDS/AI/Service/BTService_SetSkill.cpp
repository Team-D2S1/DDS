// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Service/BTService_SetSkill.h"

#include "AIController.h"
#include "AI/Skills/MonsterSkillBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Monster/MonsterBase.h"
#include "Components/Combat/MonsterCombatComponent.h"


void UBTService_SetSkill::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(!Monster) return;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	float CurrentDistance = Blackboard->GetValueAsFloat("CurrentDistance");

	// 사용 가능한 스킬들을 전부 추가한다
	TArray<UMonsterSkillBase*> UsableSkill;
	for(auto MonsterSkill : Monster->GetMonsterCombatComponent()->GetMonsterSkills())
	{
		if(MonsterSkill->GetSkillDistance() <= CurrentDistance)
		{
			if(MonsterSkill->GetIsSkillUsable())
			{
				UsableSkill.Add(MonsterSkill);
			}
		}
	}

	// 사용 가능한 스킬 중 하나 랜덤으로 Set
	if(UsableSkill.Num() > 0)
	{
		int32 RandNum = FMath::RandRange(0, UsableSkill.Num() - 1);
		Blackboard->SetValueAsObject("SelectedSkill", UsableSkill[RandNum]);
	}
}
