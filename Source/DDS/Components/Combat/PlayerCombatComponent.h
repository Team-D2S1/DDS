// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "PlayerCombatComponent.generated.h"

class ADDSSimplePlayerWeapon;
/**
 * 
 */
UCLASS()
class DDS_API UPlayerCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	ADDSSimplePlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const;
	ADDSSimplePlayerWeapon* GetPlayerCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	float GetPlayerCurrentEquippedWeaponDamageAtLevel(float InLevel = 0) const;

protected:
	// void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	
};
