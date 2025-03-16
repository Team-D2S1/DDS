// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
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
	ADDSPlayerWeapon();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponData", ReplicatedUsing=OnRep_PlayerWeaponData)
	FDDSPlayerWeaponData PlayerWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() ;
private:
	/**
	 * 무기에 의해 부여된 능력 스펙 핸들.
	 * 장착 해제시 부여 해제해야함
	 */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="WeaponData", Replicated, meta=(AllowPrivateAccess=true))
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
	

private:
	UFUNCTION()
	void OnRep_PlayerWeaponData();
	
};
