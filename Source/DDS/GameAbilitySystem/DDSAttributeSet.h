// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "DDSAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class UPawnUIComponent;
class IPawnUIInterface;

template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

UCLASS()
class DDS_API UDDSAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UDDSAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	// virtual void PostInitProperties() override;

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagToAttributeMap;

	// Default Attributes

	// 레벨
	UPROPERTY(ReplicatedUsing= OnRep_Level, BlueprintReadOnly, Category = "Default Attributes")
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Level);

	// 에너지
	UPROPERTY(ReplicatedUsing= OnRep_Energy, BlueprintReadOnly, Category = "Default Attributes")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Energy);

	// 필요 에너지(경험치 필요량)
	UPROPERTY(ReplicatedUsing= OnRep_RequireEnergy, BlueprintReadOnly, Category = "Default Attributes")
	FGameplayAttributeData RequireEnergy;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, RequireEnergy);

	// 소울 (돈)
	UPROPERTY(ReplicatedUsing= OnRep_Soul, BlueprintReadOnly, Category = "Default Attributes")
	FGameplayAttributeData Soul;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Soul);
	
	// Primary Attributes
	// 1차 능력치는 레벨업시 부여하여 증가 가능

	// (1차) 체력
	UPROPERTY(ReplicatedUsing= OnRep_Vitality, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Vitality);

	// (1차) 지구력
	UPROPERTY(ReplicatedUsing= OnRep_Endurance, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Endurance);

	// (1차) 힘
	UPROPERTY(ReplicatedUsing= OnRep_Strength, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Strength);

	// (1차) 기량
	UPROPERTY(ReplicatedUsing= OnRep_Dexterity, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Dexterity);

	// (1차) 마력
	UPROPERTY(ReplicatedUsing= OnRep_Magic, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Magic;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Magic);

	// Status Attributes

	// (상태) 체력
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Status Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Health);

	// (상태) 스태미나
	UPROPERTY(ReplicatedUsing = OnRep_Stamina, BlueprintReadOnly, Category = "Status Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Stamina);

	// (상태) 마나
	UPROPERTY(ReplicatedUsing = OnRep_Mana, BlueprintReadOnly, Category = "Status Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Mana);

	// (상태, 2차) 최대 체력
	UPROPERTY(ReplicatedUsing = OnRep_HealthMax, BlueprintReadOnly, Category = "Status Attributes")
	FGameplayAttributeData HealthMax;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, HealthMax);

	// (상태, 2차) 최대 스태미나
	UPROPERTY(ReplicatedUsing= OnRep_StaminaMax, BlueprintReadOnly, Category = "Status Attributes")
	FGameplayAttributeData StaminaMax;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, StaminaMax);

	// (상태, 2차) 최대 마나
	UPROPERTY(ReplicatedUsing= OnRep_ManaMax, BlueprintReadOnly, Category = "Status Attributes")
	FGameplayAttributeData ManaMax;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, ManaMax);

	// Offense Attributes

	// (공격력, 무기) BaseAttack
	UPROPERTY(ReplicatedUsing= OnRep_BaseAttack, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData BaseAttack;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, BaseAttack);

	// (공격력, 2차) StrAR
	UPROPERTY(ReplicatedUsing= OnRep_StrengthAR, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData StrengthAR;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, StrengthAR);
	
	// (공격력, 2차) StrAR Plus (증가량)
	UPROPERTY(ReplicatedUsing= OnRep_StrengthARPlus, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData StrengthARPlus;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, StrengthARPlus);
	
	// (공격력, 2차) DexAR
	UPROPERTY(ReplicatedUsing= OnRep_DexterityAR, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData DexterityAR;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, DexterityAR);
	
	// (공격력, 2차) DexAR Plus (증가량)
	UPROPERTY(ReplicatedUsing= OnRep_DexterityARPlus, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData DexterityARPlus;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, DexterityARPlus);
	
	// (공격력, 2차) MagicAR
	UPROPERTY(ReplicatedUsing= OnRep_MagicAR, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData MagicAR;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, MagicAR);
	
	// (공격력, 2차) MagicAR Plus (증가량)
	UPROPERTY(ReplicatedUsing= OnRep_MagicARPlus, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData MagicARPlus;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, MagicARPlus);

	// (공격력, 2차) 물리 ASR (Physical Attack Skill Ratio)
	UPROPERTY(ReplicatedUsing= OnRep_PhysicalASR, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData PhysicalASR;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, PhysicalASR);
	
	// (공격력, 2차) 기량 ASR (Dexterity Attack Skill Ratio)
	UPROPERTY(ReplicatedUsing= OnRep_DexterityASR, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData DexterityASR;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, DexterityASR);
	
	// (공격력, 2차) 마법 ASR (Magic Attack Skill Ratio)
	UPROPERTY(ReplicatedUsing= OnRep_MagicASR, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData MagicASR;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, MagicASR);

	// (공격력, 2차) 물리 공격력
	UPROPERTY(ReplicatedUsing= OnRep_AttackPower, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, AttackPower);

	// (공격력, 2차) 마법 공격력
	UPROPERTY(ReplicatedUsing= OnRep_MagicPower, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData MagicPower;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, MagicPower);

	// Defense Attributes

	// (방어력, 2차) 물리 방어력
	UPROPERTY(ReplicatedUsing= OnRep_PhysicalDefense, BlueprintReadOnly, Category = "Defense Attributes")
	FGameplayAttributeData PhysicalDefense;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, PhysicalDefense);

	// (방어력, 2차) 마법 방어력
	UPROPERTY(ReplicatedUsing= OnRep_MagicDefense, BlueprintReadOnly, Category = "Defense Attributes")
	FGameplayAttributeData MagicDefense;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, MagicDefense);

	// (방어력, 2차) 물리 저항력
	UPROPERTY(ReplicatedUsing= OnRep_PhysicalResist, BlueprintReadOnly, Category = "Defense Attributes")
	FGameplayAttributeData PhysicalResist;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, PhysicalResist);

	// (방어력, 2차) 마법 저항력
	UPROPERTY(ReplicatedUsing= OnRep_MagicResist, BlueprintReadOnly, Category = "Defense Attributes")
	FGameplayAttributeData MagicResist;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, MagicResist);


	// Equip Attributes
	// 장비 보정 전용 (합산 대상)
	// 서버에서만 관리, 복제 안함 (어차피 Effect가 복제되므로)
	
	UPROPERTY(BlueprintReadOnly) FGameplayAttributeData Equip_HealthMax;
	UPROPERTY(BlueprintReadOnly) FGameplayAttributeData Equip_StaminaMax;
	UPROPERTY(BlueprintReadOnly) FGameplayAttributeData Equip_ManaMax;

	UPROPERTY(BlueprintReadOnly) FGameplayAttributeData Equip_AttackPower;
	UPROPERTY(BlueprintReadOnly) FGameplayAttributeData Equip_MagicPower;

	UPROPERTY(BlueprintReadOnly) FGameplayAttributeData Equip_PhysicalDefense;
	UPROPERTY(BlueprintReadOnly) FGameplayAttributeData Equip_MagicDefense;
	UPROPERTY(BlueprintReadOnly) FGameplayAttributeData Equip_PhysicalResist;
	UPROPERTY(BlueprintReadOnly) FGameplayAttributeData Equip_MagicResist;

	

	// Combat Attributes
	// Damage Taken은 복제되면 안됨. 서버에서만 관리
	UPROPERTY(BlueprintReadOnly, Category = "Combat Attributes")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, DamageTaken);

	// Weapon Attributes
	// 무기 장착으로 인한 스탯 (무기별 스태미나 소모량 등)
	
	// 공격 시 필요한 스태미나
	UPROPERTY(ReplicatedUsing=OnRep_AttackRequireStamina, BlueprintReadOnly, Category = "Weapon Attributes")
	FGameplayAttributeData AttackRequireStamina;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, AttackRequireStamina);



	// OnRep Functions
	UFUNCTION()
	void OnRep_Level(const FGameplayAttributeData& OldLevel) const;
	UFUNCTION()
	void OnRep_Energy(const FGameplayAttributeData& OldEnergy) const;
	UFUNCTION()
	void OnRep_RequireEnergy(const FGameplayAttributeData& OldRequireEnergy) const;
	UFUNCTION()
	void OnRep_Soul(const FGameplayAttributeData& OldSoul) const;
	UFUNCTION()
	void OnRep_Vitality(const FGameplayAttributeData& OldVitality) const;
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	UFUNCTION()
	void OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const;
	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;
	UFUNCTION()
	void OnRep_Magic(const FGameplayAttributeData& OldMagic) const;
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_HealthMax(const FGameplayAttributeData& OldHealthMax) const;
	UFUNCTION()
	void OnRep_StaminaMax(const FGameplayAttributeData& OldStaminaMax) const;
	UFUNCTION()
	void OnRep_ManaMax(const FGameplayAttributeData& OldManaMax) const;
	UFUNCTION()
	void OnRep_BaseAttack(const FGameplayAttributeData& OldBaseAttack) const;
	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const;
	UFUNCTION()
	void OnRep_MagicPower(const FGameplayAttributeData& OldMagicPower) const;
	UFUNCTION()
	void OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense) const;
	UFUNCTION()
	void OnRep_MagicDefense(const FGameplayAttributeData& OldMagicDefense) const;
	UFUNCTION()
	void OnRep_PhysicalResist(const FGameplayAttributeData& OldPhysicalResist) const;
	UFUNCTION()
	void OnRep_MagicResist(const FGameplayAttributeData& OldMagicResist) const;
	UFUNCTION()
	void OnRep_DamageTaken(const FGameplayAttributeData& OldDamageTaken) const;
	UFUNCTION()
	void OnRep_StrengthAR(const FGameplayAttributeData& OldStrengthAR) const;
	UFUNCTION()
	void OnRep_StrengthARPlus(const FGameplayAttributeData& OldStrengthARPlus) const;
	UFUNCTION()
	void OnRep_DexterityAR(const FGameplayAttributeData& OldDexterityAR) const;
	UFUNCTION()
	void OnRep_DexterityARPlus(const FGameplayAttributeData& OldDexterityARPlus) const;
	UFUNCTION()
	void OnRep_MagicAR(const FGameplayAttributeData& OldMagicAR) const;
	UFUNCTION()
	void OnRep_MagicARPlus(const FGameplayAttributeData& OldMagicARPlus) const;
	UFUNCTION()
	void OnRep_PhysicalASR(const FGameplayAttributeData& OldPhysicalASR) const;
	UFUNCTION()
	void OnRep_DexterityASR(const FGameplayAttributeData& OldDexterityASR) const;
	UFUNCTION()
	void OnRep_MagicASR(const FGameplayAttributeData& OldMagicASR) const;
	UFUNCTION()
	void OnRep_AttackRequireStamina(const FGameplayAttributeData& OldAttackRequireStamina) const;

	// 디버깅용
	UFUNCTION(BlueprintCallable, Category = "DDS|Attributes")
	void PrintAllAttributes() const;
	
protected:
	// UPawnUIComponent* GetPawnUIComponent() const;
private:
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;
};