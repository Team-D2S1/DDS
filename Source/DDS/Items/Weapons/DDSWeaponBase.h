// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "DDSWeaponBase.generated.h"

class UBoxComponent;

UCLASS()
class DDS_API ADDSWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDSWeaponBase();

protected:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="DDS|Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="DDS|Weapon")
	TObjectPtr<UBoxComponent> WeaponCollisionBox;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="DDS|Weapon",ReplicatedUsing= OnRep_OwnerPawn)
	TObjectPtr<APawn> OwnerPawn;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="DDS|Weapon",ReplicatedUsing = OnRep_WeaponTag)
	FGameplayTag WeaponTag;
public:
	FORCEINLINE UBoxComponent* GetWeaponCollsionBox() const {return WeaponCollisionBox;};

	UFUNCTION(BlueprintCallable,Category="DDS|Weapon")
	void SetOwnerPawn(APawn* InOwnerPawn);

	UFUNCTION(BlueprintCallable,Category="DDS|Weapon")
	APawn* GetOwnerPawn() const {return OwnerPawn;};

	UFUNCTION(BlueprintCallable,Category="DDS|Weapon")
	virtual void OnRep_OwnerPawn();

	UFUNCTION(BlueprintCallable,Category="DDS|Weapon")
	void SetWeaponTag(FGameplayTag InWeaponTag);

	UFUNCTION(BlueprintCallable,Category="DDS|Weapon")
	virtual void OnRep_WeaponTag();
};
