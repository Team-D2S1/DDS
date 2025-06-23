// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "MonsterCombatComponent.generated.h"

class UMonsterSkillBase;

USTRUCT(BlueprintType)
struct FMonsterSkillInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMonsterSkillBase> MonsterSkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUsable = false;
};

UCLASS()
class DDS_API UMonsterCombatComponent : public UPlayerCombatComponent
{
	GENERATED_BODY()
	
public:
	UMonsterCombatComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TObjectPtr<UAnimMontage> DeathMontage;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<FMonsterSkillInfo> MonsterSkillClassInfos;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<TObjectPtr<UMonsterSkillBase>> MonsterSkills;
	
public:
	FORCEINLINE const TArray<UMonsterSkillBase*>& GetMonsterSkills() const { return MonsterSkills; }
};
