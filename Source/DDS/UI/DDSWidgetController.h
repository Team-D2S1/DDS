// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DDSWidgetController.generated.h"

class UPlayerCombatComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityInfo, float);


class UAttributeSet;
class UAbilitySystemComponent;
class ADDSPlayerState;
class UDDSAttributeSet;
class UDDSAbilitySystemComponent;
class AInGamePlayerController;
class ADDSHUD;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};


/**
 * <summary>
 * DDS의 인게임 UI Controller
 * 인벤토리/스탯창 등등 관리
 * </summary>
 * Overlay와 머리위에 뜨는 체력바는 PawnUIComponent에서 관리
 * 
 * Aura 프로젝트에서는 중간 연결체로 사용하지만,
 * Warrior 프로젝트에서는 PawnUIComponent가 직접 PlayerController를 참조함
 * <seealso cref="/"/>
 */
UCLASS(BlueprintType, Blueprintable)
class DDS_API UDDSWidgetController : public UObject
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParms(FWidgetControllerParams InParams);

	virtual void BroadcastInitialValue();
	virtual void BindCallbacksToDependencies();

	UFUNCTION(BlueprintPure)
	UDDSAttributeSet* GetDDSAttributeSet();
	
	UFUNCTION(BlueprintPure)
	UDDSAbilitySystemComponent* GetDDSAbilitySystemComponent();
	
	UFUNCTION(BlueprintPure)
	AInGamePlayerController* GetDDSPlayerController();
	
	UFUNCTION(BlueprintPure)
	ADDSPlayerState* GetDDSPlayerState();
	
	UFUNCTION(BlueprintPure)
	ADDSHUD* GetDDSHUD();

	UFUNCTION(BlueprintPure)
    UPlayerCombatComponent* GetPlayerCombatComponent();

	UFUNCTION(BlueprintPure, Category = "DDS|WidgetController|Crafting")
	UInventoryComponent* GetInventoryComponent() const;
protected:
	UPROPERTY(BlueprintReadOnly, Category="DDS|WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="DDS|WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="DDS|WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="DDS|WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<UDDSAbilitySystemComponent> DDSAbilitySystemComponent;
	
	UPROPERTY()
	UDDSAttributeSet* DDSAttributeSet;

	UPROPERTY()
	TObjectPtr<AInGamePlayerController> InGamePlayerController;

	UPROPERTY()
	TObjectPtr<ADDSPlayerState> DDSPlayerState;

	UPROPERTY()
	TWeakObjectPtr<ADDSHUD> ChachedDDSHUD;
	
};
