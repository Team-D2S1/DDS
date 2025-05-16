// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "DDSTypes/DDSStructTypes.h"
#include "DDSWeaponBase.h"
#include "DDSSimplePlayerWeapon.generated.h"

/**
 * 조합없이 만들어진 무기
 */
UCLASS()
class DDS_API ADDSSimplePlayerWeapon : public ADDSWeaponBase
{
	GENERATED_BODY()
public:
	ADDSSimplePlayerWeapon();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponData", ReplicatedUsing=OnRep_PlayerWeaponData)
	FDDSSimpleWeaponData PlayerWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles();
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
