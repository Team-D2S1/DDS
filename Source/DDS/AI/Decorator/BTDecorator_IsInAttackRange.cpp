// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Decorator/BTDecorator_IsInAttackRange.h"

#include "AIController.h"
#include "AI/AIController/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = "CanAttack";
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	APawn* MyPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!MyPawn) return false;
	
	ACharacter* Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	if(!Target) return false;

	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner()->GetPawn()->GetController());
	if(!AIController) return false;

	if(AttackRangeType == EAttackRangeType::Normal)
	{
		if(MyPawn->GetDistanceTo(Target) <= AIController->GetAttackRange())
		{
			return true;
		}
	}
	
	return false;
}

