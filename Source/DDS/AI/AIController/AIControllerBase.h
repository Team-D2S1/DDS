// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "AIControllerBase.generated.h"

class UDDSPerceptionComponent;
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

	// ~ Begin IGenericTeamAgentInterface interface
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override; // 다른 액터에 대한 적대 설정
	// ~ End IGenericTeamAgentInterface interface
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBlackboardData> BlackBoardData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UDDSPerceptionComponent> AIPerception;

	UPROPERTY(VisibleAnywhere,Category = "Stat")
	TObjectPtr<UAbilitySystemComponent> AbilityComponent;

	UFUNCTION(BlueprintCallable)
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// 어그로 관련.. 감지된 Actor와 어그로 순위 (높은 대상을 우선해서 쫓거나 공격)
	TArray<TPair<AActor*, float>> DetectedActors; 
	
	// 이동 관련
	UPROPERTY(VisibleAnywhere, Category = "AI Movement")
	FVector OriginPosition;
	UPROPERTY(EditDefaultsOnly, Category = "AI Movement")
	float PatrolRadius;

private:
	FGenericTeamId TeamID;
};
