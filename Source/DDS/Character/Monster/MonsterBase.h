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

protected:
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;	

	/** 몬스터 전투 관리하는 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UMonsterCombatComponent* MonsterCombatComponent;

	/** 몬스터와 관련된 UI를 관리하는 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UMonsterUIComponent> MonsterUIComponent;

	/** 몬스터 체력바 위젯 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> MonsterHealthWidgetComponent;

	/** 몬스터 초기 스탯 초기화 */
	void InitMonsterStartUpData();
	
	/** 현재 해당 몬스터가 Lock On 중인지를 표시하는 로컬 변수. 복제되면 안됨 */
	bool bIsFocused = false;
	
public:
	// IFocusable interface
	/** 플레이어가 몬스터를 타게팅했을 때 호출되는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Focusable")
	virtual void OnFocus() override;
	/** 몬스터 타게팅이 해제되었을 때 호출되는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Focusable")
	virtual void OnFocusLost() override;
	// End of IFocusable interface

	FORCEINLINE UMonsterCombatComponent* GetMonsterCombatComponent() const { return MonsterCombatComponent; }
	virtual UPawnCombatComponent* GetCombatComponent() const override;

	/* IPawnUIInterface Begin~ */
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UMonsterUIComponent* GetMonsterUIComponent() const override;
	/* ~ IPawnUIInterface End */

};

