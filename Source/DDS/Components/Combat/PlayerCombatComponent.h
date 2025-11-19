// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "DDSTypes/DDSStructTypes.h"
#include "PlayerCombatComponent.generated.h"


class ADDSCraftedPlayerWeapon;
class UDDSPlayerGameplayAbility;
class UItemInstance;
class ADDSSimplePlayerWeapon;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class DDS_API UPlayerCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	ADDSCraftedPlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const;
	ADDSCraftedPlayerWeapon* GetPlayerCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	void RegisterSpawnedWeaponById(int32 ItemId);

	
	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	float GetPlayerCurrentEquippedWeaponDamageAtLevel(float InLevel = 0) const;
	
	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	void NotifyRightWeaponChanged(UItemInstance* NewWeapon);

	
	
	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	UItemInstance* GetRightWeaponItem() const { return rightWeaponItem; }

	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	void TriggerStopStaminaRegen();

	// 구르기(회피) 및 백스텝 트리거용 헬퍼 함수
	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	void TriggerDodge(const FVector2D& MoveInput);

	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	void TriggerBackstep();

	// GameplayEffect class to apply when stopping actions to block stamina regen for 3 seconds
	UPROPERTY(EditDefaultsOnly, Category = "DDS|Combat")
	TSubclassOf<UGameplayEffect> StopStaminaRegenEffectClass;
protected: 
	// void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UItemInstance* rightWeaponItem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "DDS|Combat")
	TSubclassOf<UDDSPlayerGameplayAbility> SpawnCraftedWeaponAbilityClass;

	UPROPERTY(EditDefaultsOnly, Category = "DDS|Combat")
 	TSubclassOf<UDDSPlayerGameplayAbility> DespawnCraftedWeaponAbilityClass;
	
private:
	
};
