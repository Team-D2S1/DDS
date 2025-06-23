// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolRoute.generated.h"

class USplineComponent;

UCLASS()
class DDS_API APatrolRoute : public AActor
{
	GENERATED_BODY()
public:
	APatrolRoute();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Route")
	USplineComponent* SplineComponent;

public:
	USplineComponent* GetSplineComponent() const { return SplineComponent; }
};
