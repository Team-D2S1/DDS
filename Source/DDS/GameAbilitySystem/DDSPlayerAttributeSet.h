// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/DDSAttributeSet.h"
#include "DDSPlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class DDS_API UDDSPlayerAttributeSet : public UDDSAttributeSet
{
	GENERATED_BODY()
public:

	UDDSPlayerAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
