// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/DDSPerceptionComponent.h"

#include "Perception/AISenseConfig_Sight.h"

UDDSPerceptionComponent::UDDSPerceptionComponent()
{
	Config_FrontSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("FrontSight"));
}

void UDDSPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
	InitPerception();
}

void UDDSPerceptionComponent::InitPerception()
{
	Config_FrontSight->PeripheralVisionAngleDegrees  = 60.f;
	Config_FrontSight->DetectionByAffiliation.bDetectEnemies = true;
	Config_FrontSight->DetectionByAffiliation.bDetectFriendlies = true;
	Config_FrontSight->DetectionByAffiliation.bDetectNeutrals = true;

	Config_FrontSight->SightRadius = FrontDetectionRange;
	Config_FrontSight->LoseSightRadius = FrontDetectionRange + 100.f;
	
	ConfigureSense(*Config_FrontSight);
	SetDominantSense(Config_FrontSight->GetSenseImplementation());
}

