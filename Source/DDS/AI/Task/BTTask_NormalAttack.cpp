// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Task/BTTask_NormalAttack.h"

#include "AI/AIController/AIControllerBase.h"
#include "Components/Combat/MonsterCombatComponent.h"
#include "ETC/CustomLog.h"


UBTTask_NormalAttack::UBTTask_NormalAttack()
{
	NodeName = "DoNormalAttack";
}

EBTNodeResult::Type UBTTask_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* Controller = Cast<AAIControllerBase>(OwnerComp.GetAIOwner()->GetInstigatorController());
	if(!Controller) return EBTNodeResult::Failed;

	Controller->GetCombatComponent()->Attack();
	
	return EBTNodeResult::Succeeded;
}
