// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EntityBase.generated.h"

UCLASS()
class DDS_API AEntityBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEntityBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
};
