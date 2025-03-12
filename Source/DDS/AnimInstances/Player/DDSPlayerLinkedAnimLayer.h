// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/DDSBaseAnimInstance.h"
#include "DDSPlayerLinkedAnimLayer.generated.h"

class UDDSPlayerAnimInstance;


/**
 * 무기 애니메이션 레이어
 */
UCLASS()
class DDS_API UDDSPlayerLinkedAnimLayer : public UDDSBaseAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure,meta = (BlueprintThreadSafe))
	UDDSPlayerAnimInstance* GetPlayerAnimInstance() const;
};
