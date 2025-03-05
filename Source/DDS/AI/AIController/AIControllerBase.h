// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

class UAISenseConfig_Damage;
class UAISenseConfig_Hearing;
class UAISenseConfig_Sight;
struct FAIStimulus;
class UAbilitySystemComponent;


UCLASS()
class DDS_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerBase(FObjectInitializer const& ObjectInitializer);
	
	virtual void BeginPlay() override;

	void SetupPerception();

	virtual void InitPerception();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBlackboardData> BlackBoardData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY(VisibleAnywhere,Category = "Stat")
	TObjectPtr<UAbilitySystemComponent> AbilityComponent;

	UFUNCTION(BlueprintCallable)
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// 어그로 관련.. 감지된 Actor와 어그로 순위 (높은 대상을 우선해서 쫓거나 공격)
	TArray<TPair<AActor*, float>> DetectedActors; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Agressive")
	float FrontDetectionRange;
	UPROPERTY(EditDefaultsOnly, Category = "Agressive")
	float RearDetectionRange;
	UPROPERTY(EditDefaultsOnly, Category = "Agressive")
	float SoundDetectionRange;

	UPROPERTY(EditDefaultsOnly, Category = "Agressive")
	TObjectPtr<UAISenseConfig_Sight> Config_FrontSight;
	UPROPERTY(EditDefaultsOnly, Category = "Agressive")
	TObjectPtr<UAISenseConfig_Sight> Config_RearSight;
	UPROPERTY(EditDefaultsOnly, Category = "Agressive")
	TObjectPtr<UAISenseConfig_Hearing> Config_Hearing;
	UPROPERTY(EditDefaultsOnly, Category = "Agressive")
	TObjectPtr<UAISenseConfig_Damage> Config_Damage;
	
	// 이동 관련
	UPROPERTY(VisibleAnywhere, Category = "AI Movement")
	FVector OriginPosition;
	UPROPERTY(EditDefaultsOnly, Category = "AI Movement")
	float PatrolRadius;
};
