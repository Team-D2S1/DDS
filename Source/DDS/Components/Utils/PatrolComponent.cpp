// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Utils/PatrolComponent.h"

#include "Actor/PatrolRoute.h"
#include "Components/SplineComponent.h"

bool UPatrolComponent::GetNextDestination(FVector& NextDestination)
{
	if(PatrolRoute)
	{
		if(const USplineComponent* SplineComponent = PatrolRoute->GetSplineComponent())
		{
			NextDestination = SplineComponent->GetLocationAtSplinePoint(DestIdx, ESplineCoordinateSpace::World);
			SetDestIdxToNext();
			return true;
		}
	}

	return false;
}

void UPatrolComponent::SetDestIdxToNext()
{
	if(!PatrolRoute) return;

	if(const USplineComponent* SplineComponent = PatrolRoute->GetSplineComponent())
	{
		bool bClosedLoop = SplineComponent->IsClosedLoop();
		int32 MaxPoint = SplineComponent->GetNumberOfSplinePoints();
		if(!bReverse)
		{
			DestIdx++;
			if(DestIdx >= MaxPoint)
			{
				DestIdx = bClosedLoop ? 0 : DestIdx-2;
				if(!bClosedLoop) bReverse = true;
			}
		}
		else
		{
			DestIdx--;
			if(DestIdx < 0)
			{
				DestIdx += 2;
				bReverse = false;
			}
		}
	}
}
