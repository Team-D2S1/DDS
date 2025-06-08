// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/Abilities/DDSMonsterGameplayAbility.h"
#include "UObject/NoExportTypes.h"
#include "MonsterSkillBase.generated.h"


/*
[스킬 쿨타임 구현]
스킬 Activate에 성공했다면 스킬 실행 시 Duration형 GameplayEffect를 추가한다.
GE_Cooldown_Fireball같은 GE를 생성한다
DurationPolicy : HasDuration
DurationMagnitude : ScalableFloat
GrantedTags : Cooldown.Fireball
InhibitedTags : .. 보통 Ability 자체에서 이 태그를 블록처리

GA에는 CooldownGameplayEffectClass라는 내부 변수가 있는데, 여기에 GE를 할당한다
이러면 간편하게 쿨다운 이펙트 구현 완료

그럼 몬스터가 해야 하는 작업을 다시 생각해보자

필수 조건 : Target이 인식되어 있을 것
-> 매 틱마다 플레이어와의 거리 재계산

몬스터 스킬이 4개이고 각각 사정거리가 100, 300, 500, 700이라 해보자
현재 몬스터는 플레이어를 먼 거리에서 추격한다고 가정한다.
700 스킬이 쿨타임일 때는 플레이어 500 거리 안에 들어가면 500 스킬을 사용한다
700, 500 스킬이 쿨타임일 때는 플레이어 300 거리 안에 들어가면 스킬을 사용한다.
현재 플레이어와 300 거리 내에 있고, 300, 500, 700 거리 스킬이 전부 사용 가능하면 이 중 랜덤 스킬을 사용한다
스킬들은 각각의 쿨타임이 존재하며, 스킬들은 몬스터 안에 GA 블루프린트 리스트로 구현되어있다

블랙보드 노드로 다음에 사용할 스킬을 저장한다.
결정되면 할당되며, 할당 즉시 사용된다. 즉, 대부분의 시간 nullptr로 존재한다.
현재 스킬 사용이 가능한 상태라면, 매 틱 사용 가능한 스킬을 판단하고 SelectedSkill에 할당한다.
만약 SelectedSkill이 존재하지 않고, 현재 스킬 사용 중도 아니라면 공격 수단이 없는 상태로 판단하고 플레이어와 거리를 벌린다.
 */

UENUM()
enum class ESkillType : uint8
{
	NormalAttack,
	SkillAttack,
	MagicAttack,
};

UCLASS(Blueprintable, BlueprintType)
class DDS_API UMonsterSkillBase : public UDDSMonsterGameplayAbility
{
	GENERATED_BODY()

public:
	UMonsterSkillBase();
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void Tick(float DeltaTime);

protected:
	// 스킬 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	ESkillType SkillType = ESkillType::NormalAttack;

	// 스킬 사정거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillDistance = 100.f;

	// 스킬 사용 & 쿨타임 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillCooldown = 1.f;
	UPROPERTY(VisibleAnywhere, Category = "Skill")
	float CurrentSkillCooldown = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Skill")
	bool bIsSkillReady = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillActivateTime = 1.f;
	UPROPERTY(VisibleAnywhere, Category = "Skill")
	bool bIsSkillActivate = true;

	// 스킬 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage> SkillMontage;	

// Getter & Setters
public:
	FORCEINLINE bool IsSkillUsable() const { return bIsSkillReady; }
	FORCEINLINE float GetSkillDistance() const { return SkillDistance; }
};
