// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckCanMove.generated.h"

/**
 * 
 */
UCLASS()
class DDS_API UBTDecorator_CheckCanMove : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckCanMove();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
