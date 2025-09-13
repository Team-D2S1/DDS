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
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	// virtual void PostInitProperties() override;

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagToAttributeMap;

	// Default Attributes
	UPROPERTY(ReplicatedUsing= OnRep_Level, BlueprintReadOnly, Category = "Default Attributes")
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Level);

	UPROPERTY(ReplicatedUsing= OnRep_Energy, BlueprintReadOnly, Category = "Default Attributes")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Energy);

	UPROPERTY(ReplicatedUsing= OnRep_RequireEnergy, BlueprintReadOnly, Category = "Default Attributes")
	FGameplayAttributeData RequireEnergy;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, RequireEnergy);

	UPROPERTY(ReplicatedUsing= OnRep_Soul, BlueprintReadOnly, Category = "Default Attributes")
	FGameplayAttributeData Soul;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Soul);
	
	// Primary Attributes
	UPROPERTY(ReplicatedUsing= OnRep_Vitality, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Vitality);

	UPROPERTY(ReplicatedUsing= OnRep_Endurance, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Endurance);

	UPROPERTY(ReplicatedUsing= OnRep_Dexterity, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Dexterity);

	UPROPERTY(ReplicatedUsing= OnRep_Magic, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Magic;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Magic);

	// Status Attributes
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Status Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Health);

	UPROPERTY(ReplicatedUsing = OnRep_Stamina, BlueprintReadOnly, Category = "Status Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Stamina);

	UPROPERTY(ReplicatedUsing = OnRep_Mana, BlueprintReadOnly, Category = "Status Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Mana);

	UPROPERTY(ReplicatedUsing = OnRep_HealthMax, BlueprintReadOnly, Category = "Status Attributes")
	FGameplayAttributeData HealthMax;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, HealthMax);

	UPROPERTY(ReplicatedUsing= OnRep_StaminaMax, BlueprintReadOnly, Category = "Status Attributes")
	FGameplayAttributeData StaminaMax;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, StaminaMax);

	UPROPERTY(ReplicatedUsing= OnRep_ManaMax, BlueprintReadOnly, Category = "Status Attributes")
	FGameplayAttributeData ManaMax;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, ManaMax);

	// Offense Attributes
	UPROPERTY(ReplicatedUsing= OnRep_AttackPower, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, AttackPower);

	UPROPERTY(ReplicatedUsing= OnRep_MagicPower, BlueprintReadOnly, Category = "Offense Attributes")
	FGameplayAttributeData MagicPower;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, MagicPower);

	// Defense Attributes
	UPROPERTY(ReplicatedUsing= OnRep_PhysicalDefense, BlueprintReadOnly, Category = "Defense Attributes")
	FGameplayAttributeData PhysicalDefense;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, PhysicalDefense);

	UPROPERTY(ReplicatedUsing= OnRep_MagicDefense, BlueprintReadOnly, Category = "Defense Attributes")
	FGameplayAttributeData MagicDefense;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, MagicDefense);

	UPROPERTY(ReplicatedUsing= OnRep_PhysicalResist, BlueprintReadOnly, Category = "Defense Attributes")
	FGameplayAttributeData PhysicalResist;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, PhysicalResist);

	UPROPERTY(ReplicatedUsing= OnRep_FireResist, BlueprintReadOnly, Category = "Defense Attributes")
	FGameplayAttributeData FireResist;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, FireResist);

	// Combat Attributes
	// Damage Taken은 복제되면 안됨. 서버에서만 관리
	UPROPERTY(BlueprintReadOnly, Category = "Combat Attributes")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, DamageTaken);
	


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
	void OnRep_FireResist(const FGameplayAttributeData& OldFireResist) const;
	UFUNCTION()
	void OnRep_DamageTaken(const FGameplayAttributeData& OldDamageTaken) const;

protected:
	// UPawnUIComponent* GetPawnUIComponent() const;
private:
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;
};