// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemReplicationProxyInterface.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EntityBase.generated.h"


class UPawnCombatComponent;
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

	virtual UPawnCombatComponent* GetCombatComponent() const;
	
	UAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable,Category="DDS|Combat")
	AActor* GetFocusedObject() const { return FocusedObject; }
protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<UDDSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<UDDSAttributeSet> AttributeSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Data", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDataAsset_StartUpDataBase> EntityStartUpDataBase;
	UFUNCTION(Server,Reliable, Category = "DDS|Combat")
	virtual void Server_SetFocusedObject(AActor* InFocusedObject);
	UFUNCTION(Server,Reliable, Category = "DDS|Combat")
	virtual void Server_ClearFocusedObject();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_FocusedObject,BlueprintReadWrite)
	TObjectPtr<AActor> FocusedObject = nullptr;

	
private:
	UFUNCTION()
	virtual void OnRep_FocusedObject();
public:	
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE UDDSAbilitySystemComponent* GetDDSAbilitySystemComponent() const {return AbilitySystemComponent;}
	FORCEINLINE UDDSAttributeSet* GetDDSAttributeSet() const {return AttributeSet;}
	
};
