// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "DDSTypes/DDSStructTypes.h"
#include "GameFramework/Actor.h"
#include "DDSWeaponBase.generated.h"

class UItemInstance;
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
	
	UPROPERTY(Replicated,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
	UItemInstance* WeaponItemInstance;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponData", ReplicatedUsing=OnRep_PlayerWeaponData)
	FDDSBaseWeaponData PlayerWeaponData;

	// OnRep 용으로 존재함. 해당문제 해결되면 삭제해도됨
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="DDS|Weapon",ReplicatedUsing= OnRep_OwnerPawn)
	TObjectPtr<APawn> OwnerPawn;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="DDS|Weapon",ReplicatedUsing = OnRep_WeaponTag)
	FGameplayTag WeaponTag;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="DDS|Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="DDS|Weapon")
	TObjectPtr<UBoxComponent> WeaponCollisionBox;

	/**
	 * 무기에 의해 부여된 능력 스펙 핸들.
	 * 장착 해제시 부여 해제해야함
	 */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="WeaponData", Replicated, meta=(AllowPrivateAccess=true))
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
	
public:
	virtual UBoxComponent* GetWeaponCollsionBox() const {return WeaponCollisionBox;};
	
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

	UFUNCTION(BlueprintCallable, Category="DDS|Weapon")
	FDDSBaseWeaponData GetBaseWeaponData() const { return PlayerWeaponData; }

private:
	UFUNCTION()
	void OnRep_PlayerWeaponData();
};


