// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Inventory/InventoryComponentInterface.h"
#include "DDS/Character/EntityBase.h"
#include "Interfaces/PawnUIInterface.h"
#include "Perception/AISightTargetInterface.h"
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
class DDS_API APlayerBase : public AEntityBase, public IInventoryComponentInterface, public IAISightTargetInterface
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

	/*
	 * Implement IAISightTargetInterface
	 */
	virtual UAISense_Sight::EVisibilityResult CanBeSeenFrom(const FCanBeSeenFromContext& Context, FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested, float& OutSightStrength, int32* UserData = nullptr, const FOnPendingVisibilityQueryProcessedDelegate* Delegate = nullptr);
	

    /* IInventoryComponentInterface Begin~ */
	UFUNCTION(BlueprintCallable, Category = "DDS|InventoryComponentInterface")
    virtual UInventoryComponent* GetInventoryComponent() override;
	/* IInventoryComponentInterface End~ */

	void OnPlayerDeath();

	void OnPlayerRebirth();
	
protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	virtual void OnDeathStartTagChanged(const FGameplayTag ChangedTag, int32 NumberOfTag) override;
	virtual void OnDeathEndTagChanged(const FGameplayTag ChangedTag, int32 NumberOfTag) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;	


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerCombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "UI")
	TObjectPtr<UPlayerUIComponent> PlayerUIComponent;

	TWeakObjectPtr<UInventoryComponent> CachedInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DeathMontage;
	
public:
	
	virtual void Server_SetFocusedObject(AActor* InFocusedObject) override;
	virtual void Server_ClearFocusedObject() override;
	virtual void OnRep_FocusedObject() override;
private:
	void InitAbilityActorInfo();
private:
	// 카메라 복구 중인지 체크하는 플래그
	bool bIsResettingCamera = false;
};
