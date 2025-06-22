// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "DDSTypes/DDSStructTypes.h"
#include "DDSWeaponBase.h"
#include "DDSSimplePlayerWeapon.generated.h"

class UItemInstance;
class UItemStaticData;
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


	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles();
protected:
	// UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	// UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


	/**
	* 기본적인 무기 아이템 데이터.
	* 이 변수로 ItemInstance를 생성
	*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemStaticData> DefaultWeaponItemClass;

private:


	
	
};
