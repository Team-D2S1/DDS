// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEExecCalc_UseAttributePoint.generated.h"

/**
 * AttributePoint를 소모하여 선택된 Primary Attribute를 1 증가시키는 ExecutionCalculation
 * SetByCaller로 전달된 AttributeTag에 따라 해당 Attribute를 증가시킴
 */
UCLASS()
class DDS_API UGEExecCalc_UseAttributePoint : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEExecCalc_UseAttributePoint();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

