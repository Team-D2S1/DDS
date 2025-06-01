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

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill", meta=(AllowPrivateAccess=true))
	TArray<UMonsterSkillBase*> MonsterSkills;

public:
	FORCEINLINE const TArray<UMonsterSkillBase*>& GetMonsterSkills() const { return MonsterSkills; }
};
