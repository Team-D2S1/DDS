// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/Player/PlayerBase.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "ETC/CustomLog.h"
#include "MonsterCombatComponent.generated.h"

class AMonsterBase;
class UMonsterSkillBase;

USTRUCT(BlueprintType)
struct FMonsterSkillInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMonsterSkillBase> MonsterSkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUsable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseSkillWeight = 1.f;
};

USTRUCT(BlueprintType)
struct FCurrentSkillInfo
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UMonsterSkillBase> Skill;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float SkillWeight;

public:
	FCurrentSkillInfo() : Skill(nullptr), SkillWeight(0.f) {};
	
	FCurrentSkillInfo(const TObjectPtr<UMonsterSkillBase>& NewSkill, const float NewSkillWeight)
		: Skill(NewSkill), SkillWeight(NewSkillWeight)
	{
		if(NewSkill == nullptr || NewSkillWeight < 0.f)
		{
			MY_LOG(LogTemp,	Error, TEXT("스킬이 제대로 할당되지 않음! 혹은 가중치가 0 미만임!"));
		}
	}

	void SetSkillWeight(float NewSkillWeight)
	{
		if(NewSkillWeight < 0.f)
		{
			MY_LOG(LogTemp,	Error, TEXT("가중치가 0 미만임!"));
			return;
		}
		SkillWeight = NewSkillWeight;
	}

	TObjectPtr<UMonsterSkillBase> GetSkill() const { return Skill; }
	float GetSkillWeight() const { return SkillWeight; }
};

UCLASS()
class DDS_API UMonsterCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:
	UMonsterCombatComponent();

	void ApplyDamageToTarget(APlayerBase* Player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TObjectPtr<UAnimMontage> HitReactionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TSubclassOf<UGameplayEffect> HitEffect;
	
protected:
	virtual void BeginPlay() override;

	// 몬스터가 가지고 있는 스킬들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<FMonsterSkillInfo> MonsterSkillClassInfos;

	// 몬스터가 현재 사용 가능한 스킬
	TArray<FCurrentSkillInfo> MonsterSkills;
	
public:
	FORCEINLINE const TArray<FCurrentSkillInfo>& GetMonsterSkills() const { return MonsterSkills; }
};
