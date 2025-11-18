// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DDSTypes/DDSStructTypes.h"
#include "Items/Actor/DDSWeaponBase.h"
#include "DDSCraftedPlayerWeapon.generated.h"

class AWeaponBladePart;
class UItemInstance;
class UStaticMeshComponent;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class DDS_API ADDSCraftedPlayerWeapon : public ADDSWeaponBase
{
	GENERATED_BODY()

private:




protected:
    
	
public:
	ADDSCraftedPlayerWeapon();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	

	void SetWeaponItemInstance(UItemInstance* NewWeaponItemInstance) { WeaponItemInstance = NewWeaponItemInstance; };
	UItemInstance* GetWeaponItemInstance() const { return WeaponItemInstance; };

	virtual UBoxComponent* GetWeaponCollsionBox() const override;

	UFUNCTION(BlueprintCallable)
	void SetBladeActor(AWeaponBladePart* NewBladePartActor);
	
	UFUNCTION(BlueprintCallable)
	void SetBladeItemInstance(UItemInstance* NewBladeItemInstance);
	UFUNCTION(BlueprintCallable)
	void SetGripItemInstance(UItemInstance* NewGripItemInstance);
	// Pommel 은 더 이상 사용하지 않음
	// UFUNCTION(BlueprintCallable)
	// void SetPommelItemInstance(UItemInstance* NewPommelItemInstance);
	UFUNCTION(BlueprintCallable)
	UItemInstance* GetBladeItemInstance() const;
	UFUNCTION(BlueprintCallable)
	UItemInstance* GetGripItemInstance() const;
	// UFUNCTION(BlueprintCallable)
	// UItemInstance* GetPommelItemInstance() const;

	UFUNCTION()
	void OnBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
 	void OnBladeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Replicated)
	TObjectPtr<AWeaponBladePart> BladePartActor;

	
};
