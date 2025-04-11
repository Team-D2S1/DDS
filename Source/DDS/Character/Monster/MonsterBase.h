// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DDS/Character/EntityBase.h"
#include "Interfaces//Focusable.h"
#include "MonsterBase.generated.h"

class UWidgetComponent;
class UMonsterUIComponent;
class UMonsterCombatComponent;
class UPawnCombatComponent;

/**
 * 
 */
UCLASS()
class DDS_API AMonsterBase : public AEntityBase, public IFocusable
{
	GENERATED_BODY()
public:
	AMonsterBase();

	virtual UPawnCombatComponent* GetCombatComponent() const override;

	/* IPawnUIInterface Begin~ */
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UMonsterUIComponent* GetMonsterUIComponent() const override;
	/* ~ IPawnUIInterface End */
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UMonsterCombatComponent> MonsterCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UMonsterUIComponent> MonsterUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> MonsterHealthWidgetComponent;
	
	UFUNCTION()
	void OnRep_MonsterCombatComponent();

	// 로컬 변수. 복제되면 안됨
	bool bIsFocused = false;
	
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "UI")
	// TObjectPtr<UMonsterUIComponent> PlayerUIComponent;
private:
	void InitMonsterStartUpData();
public:
	FORCEINLINE UMonsterCombatComponent* GetMonsterCombatComponent() const { return MonsterCombatComponent; }

	// IFocusable interface
	UFUNCTION(BlueprintCallable, Category = "Focusable")
	virtual void OnFocus() override;
	UFUNCTION(BlueprintCallable, Category = "Focusable")
	virtual void OnFocusLost() override;
	// End of IFocusable interface
};

