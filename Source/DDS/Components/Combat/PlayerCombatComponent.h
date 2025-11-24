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
	void TriggerDodge(const FVector& MoveInput);

	UFUNCTION(Server, Reliable)
	void Server_TriggerDodge(FVector_NetQuantize MoveInput);

	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	void TriggerBackstep();

	UFUNCTION(Server, Reliable)
	void Server_TriggerBackstep();

	/**
	 * @brief 무기를 직접 스폰하고 등록합니다 (GA 없이)
	 * @param WeaponItemInstance 스폰할 무기의 ItemInstance
	 * @param WeaponTag 무기 태그
	 * @return 스폰된 무기 액터
	 */
	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	ADDSCraftedPlayerWeapon* SpawnAndRegisterWeapon(UItemInstance* WeaponItemInstance, FGameplayTag WeaponTag);

	/**
	 * @brief 등록된 무기를 찾아서 제거합니다
	 * @param WeaponTag 제거할 무기의 태그
	 */
	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	void DespawnAndUnregisterWeapon(FGameplayTag WeaponTag);
	void UnequipWeapon(ADDSCraftedPlayerWeapon* WeaponToUnequip);

	/**
	 * @brief 무기를 특정 소켓에 부착합니다 (모든 클라이언트에서 실행)
	 * @param Weapon 부착할 무기
	 * @param SocketName 부착할 소켓 이름
	 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AttachWeaponToSocket(ADDSCraftedPlayerWeapon* Weapon, FName SocketName);

	/**
	 * @brief 클라이언트가 안전하게 무기를 가져올 수 있는 함수 (서버에 요청 포함)
	 * @param InWeaponTag 찾을 무기의 태그
	 * @param bRequestFromServer 무기를 찾지 못했을 때 서버에 요청할지 여부
	 * @return 찾은 무기 (없으면 nullptr)
	 */
	UFUNCTION(BlueprintCallable, Category = "DDS|Combat")
	ADDSCraftedPlayerWeapon* GetPlayerCarriedWeaponByTagSafe(FGameplayTag InWeaponTag, bool bRequestFromServer = true);


	// 무기 장착 알림 RPC (서버 -> 멀티캐스트)
	UFUNCTION(Server, Reliable)
	void Server_NotifyWeaponEquipped(ADDSCraftedPlayerWeapon* EquippedWeapon);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_NotifyWeaponEquipped(ADDSCraftedPlayerWeapon* EquippedWeapon, TSubclassOf<UAnimInstance> AnimLayerClass);


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
