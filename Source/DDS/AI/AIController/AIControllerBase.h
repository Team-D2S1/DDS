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

	/** 다른 액터에 대한 ETeamAttitude를 반환 */
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	/** AI의 타겟이 새로 업데이트되었을 때 실행되는 함수 */
	UFUNCTION(BlueprintCallable)
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBlackboardData> BlackBoardData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UDDSPerceptionComponent> AIPerception;

	UPROPERTY(VisibleAnywhere,Category = "Stat")
	TObjectPtr<UAbilitySystemComponent> AbilityComponent;

	/** 감지된 Actor들과 어그로 순위 저장된 TArray. 어그로 순위가 더 높은 대상을 우선해서 공격한다 */
	TArray<TPair<AActor*, int32>> DetectedActors;

	/** 몬스터 시작 지정 위치 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior");
	FVector OriginLocation;
	/** 몬스터 행동 범위 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior");
	float BehaviorRadius;
	/** 몬스터 인식 범위 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior");
	float DetectionRadius;
	/** 몬스터 공격 가능한 범위 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior");
	float AttackRange;
};
