// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DDS/Character/EntityBase.h"
#include "MonsterBase.generated.h"

class UMonsterCombatComponent;

/**
 * 
 */
UCLASS()
class DDS_API AMonsterBase : public AEntityBase
{
	GENERATED_BODY()
public:
	AMonsterBase();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UMonsterCombatComponent> MonsterCombatComponent;
private:
	void InitMonsterStartUpData();
public:
	FORCEINLINE UMonsterCombatComponent* GetMonsterCombatComponent() const { return MonsterCombatComponent; }
};
