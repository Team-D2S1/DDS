// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/PawnExtensionComponentBase.h"
#include "PawnCombatComponent.generated.h"

// struct FGameplayTag;
class ADDSWeaponBase;

UENUM(BlueprintType)
enum EToggleCollisionType
{
	CurrentEquippedWeapon,
	LeftHand,  // 무기가 없는 경우
	RightHand, // 무기가 없는 경우
};


/**
 * 
 */
UCLASS()
class DDS_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	
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

	UPROPERTY(BlueprintReadWrite,Replicated,Category="DDS|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable,Category="DDS|Combat")
	ADDSWeaponBase* GetCurrentEquippedWeapon() const;

	/**
	 * @brief 무기 충돌을 활성화/비활성화 한다.
	 * @note 비활성화할 시, OverlappedActors를 초기화함
	 * @param bEnable 활성화 여부
	 * @param InDamageType 무기 종류
	 */
	UFUNCTION(BlueprintCallable,Category="DDS|Combat")
	void ToggleWeaponCollision(bool bEnable,EToggleCollisionType InDamageType = EToggleCollisionType::CurrentEquippedWeapon);

protected:
	
	/**
	 *  @brief 무기와 겹쳐져 있는 액터들.
	 */
	UPROPERTY()
	TArray<AActor*> OverlappedActors;
	
	virtual void OnHitTarget(AActor* InTargetActor);
	virtual void OnPulledFromTarget(AActor* InTargetActor);

private:
	TMap<FGameplayTag, ADDSWeaponBase*> CharacterCarriedWeaponMap;


};
