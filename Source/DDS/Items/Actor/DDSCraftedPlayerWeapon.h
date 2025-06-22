// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DDSTypes/DDSStructTypes.h"
#include "Items/Actor/DDSWeaponBase.h"
#include "DDSCraftedPlayerWeapon.generated.h"

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

};
