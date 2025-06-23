// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ActivateSkill.generated.h"

struct FAbilityEndedData;
class UGameplayAbility;
/**
 * 
 */
UCLASS()
class DDS_API UBTTaskNode_ActivateSkill : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_ActivateSkill();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnAbilityEnded(const FAbilityEndedData& EndedData);

	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;

	FGameplayAbilitySpecHandle CachedHandle;
	
	FDelegateHandle OnAbilityEndDelegateHandle;
};
