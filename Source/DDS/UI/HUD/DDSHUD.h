// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/Input/DataAsset_InputConfig.h"
#include "GameFramework/HUD.h"
#include "DDSHUD.generated.h"

class UGameOverUI;
class UEquipWidgetController;
class UDataAsset_InputConfig;
struct FWidgetControllerParams;
class UCraftingWidgetController;
class UDDSWidgetController;
class UDDSUserWidget;


USTRUCT(BlueprintType)
struct FWidgetSpecification
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WidgetTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDDSUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDDSWidgetController> WidgetControllerClass;
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// FGameplayTag WidgetTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;
};
/**
 * 
 */
UCLASS()
class DDS_API ADDSHUD : public AHUD
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UDDSWidgetController* GetDefualtWidgetController(const FWidgetControllerParams& params);

	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UUserWidget* ToggleMainMenuWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UUserWidget* OpenMainMenuWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	void CloseMainMenuWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UDDSWidgetController* GetMainMenuWidgetController(FWidgetControllerParams& params);

	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UUserWidget* ToggleEquipWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UUserWidget* OpenEquipWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	void CloseEquipWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UEquipWidgetController* GetEquipWidgetController(FWidgetControllerParams& params);

	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UUserWidget* ToggleInventoryWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UUserWidget* OpenInventoryWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	void CloseInventoryWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UDDSWidgetController* GetInventoryWidgetController(FWidgetControllerParams& params);

	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UUserWidget* ToggleSatatusWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UUserWidget* OpenStatusWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	void CloseStatusWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UDDSWidgetController* GetStatusWidgetController(FWidgetControllerParams& params);

	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UUserWidget* ToggleSystemWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UUserWidget* OpenSystemWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	void CloseSystemWidget();
	UFUNCTION(BlueprintCallable, Category = "DDS|HUD")
	UDDSWidgetController* GetSystemWidgetController(FWidgetControllerParams& params);

	UFUNCTION(BlueprintCallable)
	UUserWidget* ToggleWeaponCraftWidget();
	UFUNCTION(BlueprintCallable)
	UUserWidget* OpenWeaponCraftWidget();
	UFUNCTION(BlueprintCallable)
	void CloseWeaponCraftWidget();
	UFUNCTION(BlueprintCallable)
	UDDSWidgetController* GetWeaponCraftWidgetController(FWidgetControllerParams& params);
	
	UFUNCTION(BlueprintCallable)
	UUserWidget* ToggleCraftingWidget();
	UFUNCTION(BlueprintCallable)
	UUserWidget* OpenCraftingWidget();
	UFUNCTION(BlueprintCallable)
	void CloseCraftingWidget();
	UFUNCTION(BlueprintCallable)
	UCraftingWidgetController* GetCraftingWidgetController(FWidgetControllerParams& params);

	UFUNCTION(BlueprintCallable)
	void ShowGameOverWidget();
	UFUNCTION(BlueprintCallable)
	void HideGameOverWidget();
	
	/** 
	 * @brief 인풋 태그를 핸들링
	 * @param InputTag The input tag to handle.
	 * @return 핸들링 되었는지 여부.
	 */
	bool HandleInputAction(const FGameplayTag& InputTag);

protected:
	// 일단 만들어는 뒀는데 각 UI별 특이성이 있을 수 있으니 기존 함수 유지
	
	UUserWidget* OpenWidget(const FWidgetSpecification& WidgetSpec);
    UUserWidget* CloseWidget(const FWidgetSpecification& WidgetSpec);
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDDSUserWidget> CurrentOpeningWidget;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDDSWidgetController> CurrentOpeningWidgetController;

	TArray<FGameplayTag> WidgetHistory;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FWidgetSpecification MainMenuWidgetSpec;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> CurrentMainMenuWidget;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDDSWidgetController> MainMenuWidgetController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FWidgetSpecification EquipWidgetSpec;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> CurrentEquipWidget;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEquipWidgetController> EquipWidgetController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FWidgetSpecification InventoryWidgetSpec;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> CurrentInventoryWidget;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDDSWidgetController> InventoryWidgetController;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FWidgetSpecification WeaponCraftWidgetSpec;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> CurrentWeaponCraftWidget;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDDSWidgetController> WeaponCraftWidgetController;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FWidgetSpecification CraftingWidgetSpec;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> CurrentCraftingWidget;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCraftingWidgetController> CraftingWidgetController;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FWidgetSpecification StatusWidgetSpec;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> CurrentStatusWidget;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDDSWidgetController> StatusWidgetController;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FWidgetSpecification SystemWidgetSpec;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> CurrentSystemWidget;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDDSWidgetController> SystemWidgetController;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataAsset_InputConfig> InputConfig;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FWidgetSpecification GameOverWidgetSpec;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> CurrentGameOverWidget;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDDSWidgetController> GameOverWidgetController;
};

