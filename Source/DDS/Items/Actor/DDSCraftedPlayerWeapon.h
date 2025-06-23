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
	

	void SetWeaponItemInstance(UItemInstance* NewWeaponItemInstance) { WeaponItemInstance = NewWeaponItemInstance; };
	UItemInstance* GetWeaponItemInstance() const { return WeaponItemInstance; };

	virtual UBoxComponent* GetWeaponCollsionBox() const override;

	UFUNCTION(BlueprintCallable)
	void SetBladeActor(AWeaponBladePart* NewBladePartActor);
	
	UFUNCTION(BlueprintCallable)
	void SetBladeItemInstance(UItemInstance* NewBladeItemInstance);
	UFUNCTION(BlueprintCallable)
	void SetGripItemInstance(UItemInstance* NewGripItemInstance);
	UFUNCTION(BlueprintCallable)
	void SetPommelItemInstance(UItemInstance* NewPommelItemInstance);
	UFUNCTION(BlueprintCallable)
	UItemInstance* GetBladeItemInstance() const;
	UFUNCTION(BlueprintCallable)
	UItemInstance* GetGripItemInstance() const;
	UFUNCTION(BlueprintCallable)
	UItemInstance* GetPommelItemInstance() const;


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Replicated)
	TObjectPtr<AWeaponBladePart> BladePartActor;
	
};


