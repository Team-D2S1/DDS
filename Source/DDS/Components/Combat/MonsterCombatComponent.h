// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "MonsterCombatComponent.generated.h"

class UMonsterSkillBase;
/**
 * 
 */
UCLASS()
class DDS_API UMonsterCombatComponent : public UPlayerCombatComponent
{
	GENERATED_BODY()
	
public:
	UMonsterCombatComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<TSubclassOf<UMonsterSkillBase>> MonsterSkillClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<TObjectPtr<UMonsterSkillBase>> MonsterSkills;

public:
	FORCEINLINE const TArray<UMonsterSkillBase*>& GetMonsterSkills() const { return MonsterSkills; }
};
