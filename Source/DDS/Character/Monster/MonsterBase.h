// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DDS/Character/EntityBase.h"
#include "Intertaction/Focusable.h"
#include "MonsterBase.generated.h"

class UMonsterCombatComponent;

/**
 * 
 */
UCLASS()
class DDS_API AMonsterBase : public AEntityBase, public IFocusable
{
	GENERATED_BODY()
public:
	AMonsterBase();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UMonsterCombatComponent> MonsterCombatComponent;

	// 로컬 변수. 복제되면 안됨
	bool bIsFocused = false;
private:
	void InitMonsterStartUpData();
public:
	FORCEINLINE UMonsterCombatComponent* GetMonsterCombatComponent() const { return MonsterCombatComponent; }

	// IFocusable interface
	virtual void OnFocus() override;
	virtual void OnFocusLost() override;
	// End of IFocusable interface
};
