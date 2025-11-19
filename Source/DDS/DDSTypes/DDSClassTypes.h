// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DDSStructTypes.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "PaperSprite.h"
#include "GameAbilitySystem/Effects/GE_WeaponEquip.h"
#include "DDSClassTypes.generated.h"

class UGameplayEffect;
class ADDSCraftedPlayerWeapon;
class AWeaponBladePart;
class UItemInstance;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipped, UItemInstance*, ItemInstance, AActor*, EquipActor);

UCLASS(BlueprintType, Blueprintable)
class UItemStaticData : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTypeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer ItemStaticTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemTypeID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemDescription;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperSprite> ItemIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AWeaponBladePart> BladePartClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ADDSCraftedPlayerWeapon> GripPart;

	UPROPERTY(BlueprintAssignable)
	FOnItemEquipped OnItemEquipped;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> ItemEffectClass;//= UGE_WeaponEquip::StaticClass();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDDSBaseWeaponData WeaponBaseData;

	virtual bool IsSupportedForNetworking() const override { return true; }
};
