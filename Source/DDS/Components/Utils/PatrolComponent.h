// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolComponent.generated.h"


class APatrolRoute;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DDS_API UPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool GetNextDestination(FVector& NextDestination);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Route")
	APatrolRoute* PatrolRoute;

	float DestIdx = 0;
	bool bReverse = false;
	bool bIsPatrol = false;

private:
	void SetDestIdxToNext();
};
