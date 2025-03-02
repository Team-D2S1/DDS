// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameAbilitySystem/IDDSAbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "DDSPlayerState.generated.h"

class UDDSAttributeSet;
class UAbilitySystemComponent;
class UAttributeSet;

/**nnnnnnnnnnnnnn
 * 
 */
UCLASS()
class DDS_API ADDSPlayerState : public APlayerState, public IAbilitySystemInterface,public IIDDSAbilitySystemInterface
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
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<UDDSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<UDDSAttributeSet> AttributeSet;
};
