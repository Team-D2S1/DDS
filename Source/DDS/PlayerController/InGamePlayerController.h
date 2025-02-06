// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InGamePlayerController.generated.h"

class UCombatComponent;
/**
 * 
 */
UCLASS()
class DDS_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AInGamePlayerController();

protected:
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	// --- Input Action Delegate Function ---
	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Jump();

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void Dash();

	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& Value);
	
public:
	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
};