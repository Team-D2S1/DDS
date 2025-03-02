// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemReplicationProxyInterface.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EntityBase.generated.h"


class UDDSAttributeSet;
class UAttributeSet;
class UDDSAbilitySystemComponent;
class UDataAsset_StartUpDataBase;

UCLASS(Abstract) 
class DDS_API AEntityBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEntityBase();

	virtual void PossessedBy(AController* NewController) override;

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface
	UAttributeSet* GetAttributeSet() const;
protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<UDDSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<EditDefaultsOnly> AttributeSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Data", meta = (AllowPrivateAccess = "true"),ReplicatedUsing=OnRep_CharacterData)
	TSoftObjectPtr<UDataAsset_StartUpDataBase> EntityStartUpDataBase;


	UFUNCTION()
	void OnRep_CharacterData();
public:	
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE UDDSAbilitySystemComponent* GetWarriorAbilitySystemComponent() const {return AbilitySystemComponent;}
	FORCEINLINE UDDSAttributeSet* GetWarriorAttributeSet() const {return AttributeSet;}
	
};

inline void AEntityBase::OnRep_CharacterData()
{
}
