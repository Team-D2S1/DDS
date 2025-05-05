// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "DDSWeaponBase.generated.h"

class UBoxComponent;

DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*);

UCLASS()
class DDS_API ADDSWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDSWeaponBase();

	FOnTargetInteractedDelegate OnWeaponHitTarget;
	FOnTargetInteractedDelegate OnWeaponPulledFromTarget;
protected:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 이 무기의 아이템 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated,Category="DDS|Weapon")
	int32 ParentItemId;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="DDS|Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="DDS|Weapon")
	TObjectPtr<UBoxComponent> WeaponCollisionBox;

	// OnRep 용으로 존재함. 해당문제 해결되면 삭제해도됨
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


	UFUNCTION(BlueprintCallable,Category="DDS|Weapon")
	int32 GetItemId() const {return ParentItemId;};
};


