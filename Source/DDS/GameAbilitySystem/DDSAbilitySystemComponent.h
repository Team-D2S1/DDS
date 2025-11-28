// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DDSAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSLevelChangedSignature, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSEnergyChangedSignature, float, NewEnergy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSRequireEnergyChangedSignature, float, NewRequireEnergy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSStaminaChangedSignature, float, NewStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSMaxStaminaChangedSignature, float, NewMaxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSMaxManaChangedSignature, float, NewMaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSDamageTakenChangedSignature, float, NewDamageTaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSHealthPotionChangedSignature, float, NewHealthPotion);

// UI 알림용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExperienceGainedNotificationSignature, float, ExperienceAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpNotificationSignature, int32, NewLevel);

struct FDDSPlayerAbilitySet;
class UDDSGameplayAbility;
class UDDSAttributeSet;
/**
 * 
 */
UCLASS()
class DDS_API UDDSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UDDSGameplayAbility>>& StartupAbilities);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable,Category="DDS|Ability",meta=(ApplyLevel=1))
	void GrantPlayerWeaponAbilities(const TArray<FDDSPlayerAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel,TArray<FGameplayAbilitySpecHandle>& OutHandles);
	UFUNCTION(BlueprintCallable,Category="DDS|Ability")
	void RemoveGrantedPlayerWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InHandles);


	UFUNCTION(NetMulticast,Reliable)
	void Multicast_AddLooseGameplayTag(const FGameplayTag& InTag);

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_RemoveLooseGameplayTag(const FGameplayTag& InTag);
	// TODO 이펙트 적용시 클라이언트에게 알림
	// UFUNCTION(Client, Reliable)
	// void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
	/** AttributeSet 값 변경 델리게이트 바인딩 */
	void BindAttributeValueChangeDelegates(UDDSAttributeSet* InAttributeSet);


	UFUNCTION(Server, Reliable)
	void Server_UseAttributePointToAttribute(const FGameplayTag& InAttributeTag);

	/** 레벨업 함수 - 서버에서만 호출 가능 */
	UFUNCTION(BlueprintCallable, Category="DDS|Level", BlueprintAuthorityOnly)
	void LevelUp(int32 LevelsToAdd = 1);

	/** 경험치 추가 함수 - 서버에서만 호출 가능 */
	UFUNCTION(BlueprintCallable, Category="DDS|Experience", BlueprintAuthorityOnly)
	void AddExperience(float ExperienceToAdd);

	/** 경험치를 추가하고 자동으로 레벨업 체크 - 서버에서만 호출 가능 */
	UFUNCTION(BlueprintCallable, Category="DDS|Experience", BlueprintAuthorityOnly)
	void AddExperienceAndCheckLevelUp(float ExperienceToAdd);

	/** 스태미나 감소 함수*/
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="DDS|Stamina", BlueprintAuthorityOnly)
	void Server_ReduceStamina(float StaminaToReduce);

    /** 스태미나 재생 중지 함수 */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="DDS|Stamina", BlueprintAuthorityOnly)
	void Server_StopStaminaRegen();
	
	/** 현재 경험치 가져오기 */
	UFUNCTION(BlueprintPure, Category="DDS|Experience")
	float GetCurrentExperience() const;

	/** 레벨업에 필요한 경험치 가져오기 */
	UFUNCTION(BlueprintPure, Category="DDS|Experience")
	float GetRequiredExperience() const;

	/** 경험치 진행률 가져오기 (0.0 ~ 1.0) */
	UFUNCTION(BlueprintPure, Category="DDS|Experience")
	float GetExperienceProgress() const;

	// 게임 전역에서 사용 가능한 Attribute 변경 델리게이트 (UI, 이펙트, 사운드 등 모두 구독 가능)
	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSLevelChangedSignature OnLevelChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSEnergyChangedSignature OnEnergyChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSRequireEnergyChangedSignature OnRequireEnergyChanged;
	
	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSStaminaChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSMaxStaminaChangedSignature OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSManaChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSMaxManaChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSDamageTakenChangedSignature OnDamageTakenChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSHealthPotionChangedSignature OnHealthPotionChanged;

	// UI 알림용 델리게이트 (경험치 획득, 레벨업 팝업 표시용)
	UPROPERTY(BlueprintAssignable, Category="DDS|Notifications")
	FOnExperienceGainedNotificationSignature OnExperienceGainedNotification;

	UPROPERTY(BlueprintAssignable, Category="DDS|Notifications")
	FOnLevelUpNotificationSignature OnLevelUpNotification;

	/** AttributePoint를 사용하여 Attribute를 증가시키는 GameplayEffect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Attributes")
	TSubclassOf<UGameplayEffect> UseAttributePointEffectClass;

	/** Player Stats를 계산하는 MMC GameplayEffect (GE_PlayerStats_Naive) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Attributes")
	TSubclassOf<UGameplayEffect> PlayerStatsEffectClass;

	/** Player의 Stamina를 줄이는 GameplayEffect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Attributes")
	TSubclassOf<UGameplayEffect> ReduceStaminaEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Attributes")
	TSubclassOf<UGameplayEffect> StopStaminaRegenEffectClass;

protected:
	void HandleLevelChanged(const FOnAttributeChangeData& Data);
	void HandleEnergyChanged(const FOnAttributeChangeData& Data);
	void HandleRequireEnergyChanged(const FOnAttributeChangeData& Data);
	void HandleHealthChanged(const FOnAttributeChangeData& Data);
	void HandleStaminaChanged(const FOnAttributeChangeData& Data);
	void HandleManaChanged(const FOnAttributeChangeData& Data);
	void HandleHealthMaxChanged(const FOnAttributeChangeData& Data);
	void HandleStaminaMaxChanged(const FOnAttributeChangeData& Data);
	void HandleManaMaxChanged(const FOnAttributeChangeData& Data);
	void HandleDamageTakenChanged(const FOnAttributeChangeData& Data);
	void HandleHealthPotionChanged(const FOnAttributeChangeData& Data);

	// 마지막 구르기 입력 방향 (월드 좌표, 정규화 벡터) - Replicated
	UPROPERTY(Replicated, BlueprintReadOnly, Category="DDS|Ability", meta=(AllowPrivateAccess="true"))
	FVector LastDodgeInputDirection = FVector::ForwardVector;

public:
	/**
	 * Applies a GameplayEffect to self. If there are existing active effects that match the
	 * provided TagToMatch or are instances of EffectClass, they will be removed first so the
	 * new effect replaces/refreshes the duration.
	 */
	UFUNCTION(BlueprintCallable, Category="DDS|Ability")
	FActiveGameplayEffectHandle ApplyOrRefreshGameplayEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level = 1.0f, const FGameplayTag& TagToMatch = FGameplayTag());

	// 구르기 방향 캐시 세터/게터 (블루프린트에서도 사용 가능)
	UFUNCTION(BlueprintCallable, Category="DDS|Ability")
	void SetLastDodgeInputDirection(const FVector& InDirection);

	UFUNCTION(BlueprintCallable, Category="DDS|Ability")
	FVector GetLastDodgeInputDirection() const { return LastDodgeInputDirection; }


	static float GetRequiredEnergyForLevel(int32 Level);
};
