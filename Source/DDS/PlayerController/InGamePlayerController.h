// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
// #include "InputMappingContext.h"
#include "InGamePlayerController.generated.h"

class UDataAsset_InputConfig;
// class UCombatComponent;
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
	void Input_Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Input_Jump();

	UFUNCTION(BlueprintCallable)
	void Input_Look(const FInputActionValue& Value);
	
	// UFUNCTION(BlueprintCallable)
	// void Input_Attack();
	//
	// UFUNCTION(BlueprintCallable)
	// void Input_Dash();
	
public:
	// UPROPERTY()
	// TObjectPtr<UCombatComponent> CombatComponent;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Data", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;
};