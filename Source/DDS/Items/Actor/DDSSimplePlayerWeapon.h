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
protected:
	// UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	// UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
private:

	

private:
	UFUNCTION()
	void OnRep_PlayerWeaponData();
	
};
