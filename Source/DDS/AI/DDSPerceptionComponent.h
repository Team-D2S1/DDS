// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "DDSPerceptionComponent.generated.h"

class UAISenseConfig_Damage;
class UAISenseConfig_Hearing;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class DDS_API UDDSPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	UDDSPerceptionComponent();

	virtual void BeginPlay() override;
	
protected:
	void InitPerception();
	
	UPROPERTY(EditDefaultsOnly, Category = "Agressive")
	float FrontDetectionRange;
	UPROPERTY(EditDefaultsOnly, Category = "Agressive")
	float SoundDetectionRange;

	UPROPERTY(EditDefaultsOnly, Category = "Agressive")
	TObjectPtr<UAISenseConfig_Sight> Config_FrontSight;
	UPROPERTY(EditDefaultsOnly, Category = "Agressive")
	TObjectPtr<UAISenseConfig_Hearing> Config_Hearing;
	UPROPERTY(EditDefaultsOnly, Category = "Agressive")
	TObjectPtr<UAISenseConfig_Damage> Config_Damage;

public:
	TObjectPtr<UAISenseConfig_Sight> GetFrontSightConfig() { return Config_FrontSight; }
	
};
