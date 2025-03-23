// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/PawnExtensionComponentBase.h"
#include "PawnCombatComponent.generated.h"

// struct FGameplayTag;
class ADDSWeaponBase;
/**
 * 
 */
UCLASS()
class DDS_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	/**
	 * 멀티 환경에서도 구현이 되어야함
	 * @param InWeaponTag 
	 * @param InWeapon 
	 * @param bRegisterAsEquippedWeapon 등록 + 장착 여부
	 */
	UFUNCTION(BlueprintCallable,Category="DDS|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTag, ADDSWeaponBase* InWeapon,bool bRegisterAsEquippedWeapon = false);

	/**
	 * 
	 * @param InWeaponTag 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable,Category="DDS|Combat")
	ADDSWeaponBase* GetCharacterCarriedWeapon(FGameplayTag InWeaponTag) const;

	UPROPERTY(BlueprintReadWrite,Category="DDS|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable,Category="DDS|Combat")
	ADDSWeaponBase* GetCurrentEquippedWeapon() const;



protected:

private:
	TMap<FGameplayTag, ADDSWeaponBase*> CharacterCarriedWeaponMap;


};
