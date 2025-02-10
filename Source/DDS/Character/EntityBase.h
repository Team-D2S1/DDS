// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemReplicationProxyInterface.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EntityBase.generated.h"


class UAttributeSet;

UCLASS(Abstract) 
class DDS_API AEntityBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEntityBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
protected:
	virtual void BeginPlay() override;


	UPROPERTY()
	TObjectPtr< UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
public:	
	virtual void Tick(float DeltaTime) override;
	
};
