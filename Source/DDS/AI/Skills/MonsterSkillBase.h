// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MonsterSkillBase.generated.h"

UENUM()
enum class ESkillType : uint8
{
	NormalAttack,
	SkillAttack,
	MagicAttack,
};

UCLASS()
class DDS_API UMonsterSkillBase : public UObject
{
	GENERATED_BODY()

public:
	// Skill이 시작될 때 호출되는 함수
	virtual void  ActivateSkill();

	// Skill 종료될 때 불릴 함수
	virtual void DeactivateSkill();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	ESkillType SkillType = ESkillType::NormalAttack;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillDistance = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillCooldown = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	float CurrentSkillCooldown = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bIsSkillReady = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage> SkillMontage;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillActivateTime = 1.f;

// Getter & Setters
public:
	FORCEINLINE float GetSkillDistance() const { return SkillDistance; }
};
