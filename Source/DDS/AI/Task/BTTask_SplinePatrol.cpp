// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Task/BTTask_SplinePatrol.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Monster/MonsterBase.h"
#include "Components/Utils/PatrolComponent.h"


EBTNodeResult::Type UBTTask_SplinePatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(UPatrolComponent* PatrolComponent = Monster->GetPatrolComponent())
	{
		FVector NextDestination;
		bool bSuccess = PatrolComponent->GetNextDestination(NextDestination);
		if(bSuccess)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolLocation", NextDestination);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
