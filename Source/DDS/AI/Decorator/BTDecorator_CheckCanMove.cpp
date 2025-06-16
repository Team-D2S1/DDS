// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Decorator/BTDecorator_CheckCanMove.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ETC/CustomLog.h"


UBTDecorator_CheckCanMove::UBTDecorator_CheckCanMove()
{
	NodeName = "Check Can Move";
}

bool UBTDecorator_CheckCanMove::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	MY_LOG(LogTemp, Error, TEXT("거리 판정"));

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	if(Blackboard->GetValueAsBool("bIsUsingSkill") == true)
	{
		return false;
	}

	if(Blackboard->GetValueAsBool("bIsStunned") == true)
	{
		return false;
	}

	return true;
}
