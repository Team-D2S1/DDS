// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "EntityBase.h"

AEntityBase::AEntityBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEntityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEntityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

