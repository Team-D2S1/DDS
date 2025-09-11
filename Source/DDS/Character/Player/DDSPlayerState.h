// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Components/Inventory/InventoryComponentInterface.h"
#include "GameAbilitySystem/IDDSAbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "DDSPlayerState.generated.h"

struct FGameplayTagContainer;
class UInventoryComponent;
class UDDSAttributeSet;
class UDDSPlayerAttributeSet;
class UAbilitySystemComponent;
class UAttributeSet;



/**
 * 
 */
UCLASS()
class DDS_API ADDSPlayerState : public APlayerState, public IAbilitySystemInterface,public IIDDSAbilitySystemInterface, public IGenericTeamAgentInterface, public IInventoryComponentInterface
{
	GENERATED_BODY()

public:
	ADDSPlayerState();

	// UFUNCTION(Exec)
	// void PrintAllAbilities() const;


	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UDDSAbilitySystemComponent* GetDDSAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;
	UDDSAttributeSet* GetDDSAttribueSet() const;


	// ~ Begin IGenericTeamAgentInterface Interface
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	// ~ End IGenericTeamAgentInterface Interface

	// ~ Begin IInventoryComponentInterface Interface
	virtual UInventoryComponent* GetInventoryComponent() override { return InventoryComponent; }
	// ~ End IInventoryComponentInterface Interface
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<UDDSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<UDDSAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY()
	FGenericTeamId PlayerTeamId;
};
