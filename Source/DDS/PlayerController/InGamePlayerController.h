// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
// #include "InputMappingContext.h"
#include "InGamePlayerController.generated.h"

class IFocusable;
class APlayerBase;
class UDDSAbilitySystemComponent;
class UDataAsset_InputConfig;
// class UCombatComponent;
/**
 * 
 */
UCLASS()
class DDS_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AInGamePlayerController();


protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// --- Input Action Delegate Function ---
	UFUNCTION(BlueprintCallable)
	void Input_Move(const FInputActionValue& Value);

	// B(Button) 입력 처리 - 패드 B / 키보드 Space 대응
	UFUNCTION(BlueprintCallable)
	void Input_B_Pressed(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Input_B_Released(const FInputActionValue& Value);

	// UFUNCTION(BlueprintCallable)
	// void Input_Jump();

	UFUNCTION(BlueprintCallable)
	void Input_Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Input_LockOn();
	UFUNCTION(BlueprintCallable)
	void Input_UIInputPressed(FGameplayTag InputTag);
	UFUNCTION(BlueprintCallable)
	void Input_UIInputReleased(FGameplayTag InputTag);
	
	UFUNCTION(BlueprintCallable)
	void Input_AbilityInputPressed(FGameplayTag InputTag);
	UFUNCTION(BlueprintCallable)
	void Input_AbilityInputReleased(FGameplayTag InputTag);
	
	UFUNCTION(BlueprintCallable)
	void Input_Debug_PrintAttributes();

	// --- Cheat Functions for Testing ---
	UFUNCTION(BlueprintCallable, Category="DDS|Cheat")
	void Cheat_AddExp();

	UFUNCTION(BlueprintCallable, Category="DDS|Cheat")
	void Cheat_AddAttributePoints();

	UFUNCTION(BlueprintCallable, Category="DDS|Cheat")
	void Cheat_LevelUp();

	UFUNCTION(BlueprintCallable, Category="DDS|Cheat")
	void Cheat_FullHeal();

public:
	// UPROPERTY()
	// TObjectPtr<UCombatComponent> CombatComponent;

	/**
	 * 움직임, 스킬 입력 무시.
	 * UI는 무시하지 않음
	 * @param bIgnore 무시 여부
	 */
	UFUNCTION(BlueprintCallable)
	void SetIgnoreGameInput(bool bIgnore);

	UFUNCTION(BlueprintCallable)
	APlayerBase* GetPlayerBase();
protected:
	
	UDDSAbilitySystemComponent* GetDDSAbilitySystemComponent();


	TArray<AActor*> GetFocusables() const;
	// TArray<AActor*> GetFocusablesInSight() const;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Data", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	UPROPERTY()
	AActor* focusedObject; // TODO: PlayerState로 이관
	TWeakObjectPtr<APlayerBase> CachedPlayerBase;

	bool bIsIgnoringGameInput = false;

	// --- 이동/대시/구르기 입력 상태 ---
	// 이동 입력 벡터 (L 스틱)
	FVector CachedMoveVector = FVector::ZeroVector;

	// B 버튼 상태
	bool bBPressed = false;
	float BPressedTime = 0.f;

	// 짧은/긴 입력을 나누는 임계값 (초)
	UPROPERTY(EditDefaultsOnly, Category="Input|Move")
	float BShortPressThreshold = 0.05f;

	// 짧은/긴 입력을 나누는 임계값 (초)
	UPROPERTY(EditDefaultsOnly, Category="Input|Move")
	float BLongPressThreshold = 0.4f;
	
	// 이동 속도 계수
	UPROPERTY(EditDefaultsOnly, Category="Move")
	float BaseMoveSpeed = 400.f;

	UPROPERTY(EditDefaultsOnly, Category="Move")
	float SlowMoveSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, Category="Move")
	float SprintMultiplier = 3.f;

	// 현재 적용 중인 이동 속도 모드
	enum class EMoveSpeedMode : uint8
	{
		Normal,
		Slow,
		Sprint
	};

	EMoveSpeedMode CurrentMoveSpeedMode = EMoveSpeedMode::Normal;

	// 내부 헬퍼 함수들
	void UpdateMovementSpeedFromInput(const FVector2D& MoveVector);
	void ApplyMoveSpeedMode();
	void TryExecuteDodgeOrBackstep();
};
