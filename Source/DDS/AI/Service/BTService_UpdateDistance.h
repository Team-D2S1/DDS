// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateDistance.generated.h"

/**
 * 
 */
UCLASS()
class DDS_API UBTService_UpdateDistance : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateDistance();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
