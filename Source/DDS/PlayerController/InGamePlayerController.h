// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
// #include "InputMappingContext.h"
#include "InGamePlayerController.generated.h"

class IFocusable;
class APlayerBase;
class UDDSAbilitySystemComponent;
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

	// UFUNCTION(BlueprintCallable)
	// void Input_Jump();

	UFUNCTION(BlueprintCallable)
	void Input_Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Input_LockOn();
	UFUNCTION(BlueprintCallable)
	void Input_UIInputPressed(FGameplayTag InputTag);
	UFUNCTION(BlueprintCallable)
	void Input_UIInputReleased(FGameplayTag InputTag);
	
	UFUNCTION(BlueprintCallable)
	void Input_AbilityInputPressed(FGameplayTag InputTag);
	UFUNCTION(BlueprintCallable)
	void Input_AbilityInputReleased(FGameplayTag InputTag);
	
	// UFUNCTION(BlueprintCallable)
	// void Input_Attack();
	//
	// UFUNCTION(BlueprintCallable)
	// void Input_Dash();


	UFUNCTION(BlueprintCallable)
	void Input_Debug_PrintAttributes();
public:
	// UPROPERTY()
	// TObjectPtr<UCombatComponent> CombatComponent;

	/**
	 * 움직임, 스킬 입력 무시.
	 * UI는 무시하지 않음
	 * @param bIgnore 무시 여부
	 */
	UFUNCTION(BlueprintCallable)
	void SetIgnoreGameInput(bool bIgnore);

	UFUNCTION(BlueprintCallable)
	APlayerBase* GetPlayerBase();
protected:
	
	UDDSAbilitySystemComponent* GetDDSAbilitySystemComponent();


	TArray<AActor*> GetFocusables() const;
	// TArray<AActor*> GetFocusablesInSight() const;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Data", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;
	UPROPERTY()
	AActor* focusedObject; // TODO: PlayerState로 이관
	TWeakObjectPtr<APlayerBase> CachedPlayerBase;

	bool bIsIgnoringGameInput = false;
};
