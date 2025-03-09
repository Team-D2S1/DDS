// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DDSTypes/DDSStructTypes.h"
#include "Items/Weapons/DDSWeaponBase.h"
#include "DDSPlayerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class DDS_API ADDSPlayerWeapon : public ADDSWeaponBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponData")
	FDDSPlayerWeaponData PlayerWeaponData;
};
