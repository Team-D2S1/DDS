// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemReplicationProxyInterface.h"
#include "AbilitySystemInterface.h"
#include "Components/Combat/IPawnCombatInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/PawnUIInterface.h"
#include "EntityBase.generated.h"


class UDDSPlayerAttributeSet;
struct FGameplayEffectModCallbackData;
class UPawnCombatComponent;
class UDDSAttributeSet;
class UAttributeSet;
class UDDSAbilitySystemComponent;
class UDataAsset_StartUpDataBase;

UCLASS(Abstract) 
class DDS_API AEntityBase : public ACharacter, public IAbilitySystemInterface, public IIPawnCombatInterface, public IPawnUIInterface
{
	GENERATED_BODY()

public:
	AEntityBase();

	virtual void PossessedBy(AController* NewController) override;

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface

	/* IAbilitySystemReplicationProxyInterface Begin~ */
	virtual UPawnCombatComponent* GetCombatComponent() const override;
	/* ~ IAbilitySystemReplicationProxyInterface End */
	
	/* IPawnUIInterface Begin~ */
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	/* ~ IPawnUIInterface End */


	UAttributeSet* GetAttributeSet() const;
	UFUNCTION(BlueprintCallable,Category="DDS|AbilitySystem")
	UDDSPlayerAttributeSet* GetPlayerAttributeSet() const;

	UFUNCTION(BlueprintCallable,Category="DDS|Combat")
	AActor* GetFocusedObject() const { return FocusedObject; }

	UFUNCTION(BlueprintCallable,NetMulticast,Reliable)
	void Multicast_LinkAnimLayer(TSubclassOf<UAnimInstance> InAnimLayerClass);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_UnlinkAnimLayer(TSubclassOf<UAnimInstance> InAnimLayerClass);

	UFUNCTION(BlueprintCallable, Category="DDS|Data")
	EMoveDirection4 GetMoveDirection4(FVector WorldDirection) const;

	UFUNCTION(BlueprintCallable, Category="DDS|Data")
	EMoveDirection8 GetMoveDirection8(FVector WorldDirection) const;
	
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

	UPROPERTY(ReplicatedUsing=OnRep_FocusedObject,BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> FocusedObject = nullptr;

	// Death Start Callback - Play Montage, Collision Set ..
	UFUNCTION()
	virtual void OnDeathStartTagChanged(const FGameplayTag ChangedTag, int32 NumberOfTag);
	// Death End Callback - Destroy Actor
	UFUNCTION()
	virtual void OnDeathEndTagChanged(const FGameplayTag ChangedTag, int32 NumberOfTag);

	UFUNCTION()
	virtual void OnRep_FocusedObject();
	
private:
	
public:	
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE UDDSAbilitySystemComponent* GetDDSAbilitySystemComponent() const {return AbilitySystemComponent;}
	FORCEINLINE UDDSAttributeSet* GetDDSAttributeSet() const {return AttributeSet;}
	
};
