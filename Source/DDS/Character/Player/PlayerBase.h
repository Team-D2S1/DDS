// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DDS/Character/EntityBase.h"
#include "Interfaces/PawnUIInterface.h"
#include "PlayerBase.generated.h"

class IFocusable;
class UPlayerCombatComponent;
class UCameraComponent;
class USpringArmComponent;
class UPawnCombatComponent;
/**
 * 
 */
UCLASS()
class DDS_API APlayerBase : public AEntityBase
{
	GENERATED_BODY()

public:
	APlayerBase();

	// virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override; 
	virtual UPawnCombatComponent* GetCombatComponent() const override;
	
	FORCEINLINE UCameraComponent* GetCameraComponent() { return Camera; }
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() { return SpringArm; }
	FORCEINLINE UPlayerCombatComponent* GetCombatComponent() { return CombatComponent; }

	/* IPawnUIInterface Begin~ */
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UPlayerUIComponent* GetPlayerUIComponent() const override;
	/* IPawnUIInterface End~ */
protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerCombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "UI")
	TObjectPtr<UPlayerUIComponent> PlayerUIComponent;
public:
	
	virtual void Server_SetFocusedObject(AActor* InFocusedObject) override;
	virtual void Server_ClearFocusedObject() override;
private:
	void InitAbilityActorInfo();
	
};
