// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "InGamePlayerController.h"
// #include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
// #include "DDS/Character/CombatComponent/CombatComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "DDS/Character/Player/PlayerBase.h"
#include "DDS/DataAsset/Input/DataAsset_InputConfig.h"
#include "DDS/ETC/CustomLog.h"
#include "DDS/DDSGameplayTags.h"
#include "DDS/Components/Input/DDSInputComponent.h"
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameAbilitySystem/DDSAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Focusable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/HUD/DDSHUD.h"

AInGamePlayerController::AInGamePlayerController()
{
	// CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));

	
}


void AInGamePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// B 버튼이 눌려있는 동안 누른 시간 누적
	if (bBPressed)
	{
		BPressedTime += DeltaSeconds;
	}
}

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 강의에선 Setup에서만 했는데 여기서 해야할 이유가 있음?
	// UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// if(!Subsystem) return;
	//
	// Subsystem->AddMappingContext(InputMappingContext, 0);
}

void AInGamePlayerController::SetupInputComponent()
{
	checkf(InputConfigDataAsset, TEXT("Input Config Data Asset is null, cannot setup input"));
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(!Subsystem) return;

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	Subsystem->AddMappingContext(InputConfigDataAsset->UIInputMappingContext, 1);

	UDDSInputComponent* DDSInputComponent = CastChecked<UDDSInputComponent>(InputComponent);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	// DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Jump, ETriggerEvent::Started, this, &ThisClass::Input_Jump);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_LockOn, ETriggerEvent::Started, this, &ThisClass::Input_LockOn);
	// B 버튼 (패드 B / Space) 입력 바인딩
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_DodgeB, ETriggerEvent::Started, this, &ThisClass::Input_B_Pressed);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_DodgeB, ETriggerEvent::Completed, this, &ThisClass::Input_B_Released);
	
 	DDSInputComponent->BindUIActions(InputConfigDataAsset, this, &ThisClass::Input_UIInputPressed, &ThisClass::Input_UIInputReleased);
	DDSInputComponent->BindAbilityInputActions(InputConfigDataAsset,this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);


	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Debug_PrintAttributes, ETriggerEvent::Started, this, &ThisClass::Input_Debug_PrintAttributes);
	
	MY_LOG(LogTemp, Log, TEXT("Setup Input Complete"));
}

void AInGamePlayerController::Input_Move(const FInputActionValue& Value)
{
	// MY_LOG_DISPLAY("Move %s", *Value.ToString());
	FVector2D MoveVector = Value.Get<FVector2D>();
	CachedMoveVector = MoveVector;

	UpdateMovementSpeedFromInput(MoveVector);
	ApplyMoveSpeedMode();

	FRotator YawRotator(0.f, GetControlRotation().Yaw, 0.f);
	FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
	
	APawn* MyPawn = GetPawn();
	if(!MyPawn) return;
	if (MoveVector.Y != 0.f)
	{
		MyPawn->AddMovementInput(ForwardVector, MoveVector.Y);
	}
	if (MoveVector.X != 0.f)
	{
		MyPawn->AddMovementInput(RightVector, MoveVector.X);
	}
}

void AInGamePlayerController::Input_B_Pressed(const FInputActionValue& Value)
{
	// B 버튼이 눌리기 시작한 시점
	bBPressed = true;
	BPressedTime = 0.f;
}

void AInGamePlayerController::Input_B_Released(const FInputActionValue& Value)
{
	// 짧게/길게 여부는 Tick에서 BPressedTime으로 판별
	if (!bBPressed)
	{
		return;
	}

	// 눌렀다가 뗀 시점에서 구르기/백스텝 시도
	TryExecuteDodgeOrBackstep();

	bBPressed = false;
	BPressedTime = 0.f;
}

void AInGamePlayerController::Input_Look(const FInputActionValue& Value)
{
	// MY_LOG(LogTemp, Log, TEXT("Character Look"));
	if (focusedObject) // Lock On 상태에서는 카메라 회전을 막기
	{
		return;
	}
	
	FVector2D LookVector = Value.Get<FVector2D>();
	// APawn* MyPawn = GetPawn();
	// if(!MyPawn) return;
	if (LookVector.X != 0.f)
	{
		AddYawInput(LookVector.X);
	}
	if (LookVector.Y != 0.f)
	{
		AddPitchInput(-LookVector.Y);
	}
} 

// 클라에서 튕김. 
void AInGamePlayerController::Input_LockOn()
{
	if (!IsLocalController())
		return;
	if (bIsIgnoringGameInput)
		return;
	APlayerBase* PlayerBase = GetPlayerBase();
	if (!PlayerBase) return;

	MY_CLOG_DISPLAY_NET(FColor::White, HasAuthority(), TEXT("Start Lock On"));
	// 대상을 찾는다.
	// 대상은 현재 플레이어 카메라 기준 가장 가운데에 가까운 적
	// 대상이 없으면? 그냥 취소
	// UCameraComponent* Cam = PlayerBase->GetCameraComponent();
	// UPlayerCombatComponent* CombatComponent = PlayerBase->GetCombatComponent();
	AActor* target = nullptr;
	if (!focusedObject)
	{
		// float inf
		float minDistance = std::numeric_limits<float>::max();
		for (auto focusableActor : GetFocusables())
		{
			FVector2D ScreenLocation;
			if (focusableActor->WasRecentlyRendered(0.2f) && ProjectWorldLocationToScreen(focusableActor->GetActorLocation(), ScreenLocation))
			{
				IFocusable* focusable = Cast<IFocusable>(focusableActor);
				if (!focusable) continue;
				FVector2D ScreenCenter = FVector2D(0.5f, 0.5f);
				float distance = FVector2D::Distance(ScreenLocation, ScreenCenter);
				if (distance < minDistance)
				{
					minDistance = distance;
					target =  focusableActor;
				}
			}
		}
	}
	IFocusable* targetFocusable = Cast<IFocusable>(target);
	// 카메라 고정 
	// 스프링암 고정 Ok
	// 플레이어는 적만을 바라보도록
	// 이동키를 눌러도 방향은 적만을 바라봄
	//GetActorsInSelectionRectangle
	if (target)
	{
		MY_CLOG_DISPLAY_NET(FColor::White, HasAuthority(), TEXT("Lock On Target : %s"), *target->GetName());
		// USpringArmComponent* SpringArm = PlayerBase->GetSpringArmComponent();
		// SpringArm->bUsePawnControlRotation = false;
		
  
		if (focusedObject && focusedObject != target)
		{
			Cast<IFocusable>(focusedObject)->OnFocusLost();
		}
		PlayerBase->Server_SetFocusedObject(target);
		targetFocusable->OnFocus();
		focusedObject = target;
		
	}
	else
	{
		// USpringArmComponent* SpringArm = PlayerBase->GetSpringArmComponent();
		// SpringArm->bUsePawnControlRotation = true;
		
		
		if (focusedObject)
		{
	
			Cast<IFocusable>(focusedObject)->OnFocusLost();
			focusedObject = nullptr;
			PlayerBase->Server_ClearFocusedObject();
		}
	}
	
}

void AInGamePlayerController::Input_UIInputPressed(FGameplayTag InputTag)
{
	MY_LOG(LogTemp, Type::Log, TEXT("UI Input Pressed %s"), *InputTag.ToString());
	// MY_CLOG_DISPLAY_NET(FColor::Cyan,HasAuthority(), TEXT("UI Input Pressed %s"), *InputTag.ToString());
	if (!IsLocalController())
	{
		MY_LOG(LogTemp, Type::Warning, TEXT("Not Local Controller"));
		return;
	}
	// 움직임 멈추기
	// Input_Move(FInputActionValue(FVector2D(0.f, 0.f)));
	
	// 실제 UI 처리
	if (ADDSHUD * HUD = Cast<ADDSHUD>(GetHUD()))
	{
		HUD->HandleInputAction(InputTag);
	}else
	{
		MY_ERROR_DISPLAY(TEXT("HUD is nullptr"));
	}
}

void AInGamePlayerController::Input_UIInputReleased(FGameplayTag InputTag)
{
	// GetDDSAbilitySystemComponent()->UIInputTagReleased(InputTag);
}


void AInGamePlayerController::Input_AbilityInputPressed(FGameplayTag InputTag)
{
	if (bIsIgnoringGameInput)
		return;
	// MY_CLOG_DISPLAY_NET(FColor::Cyan,HasAuthority(), TEXT("Ability Input Pressed %s"), *InputTag.ToString());
	MY_LOG(LogTemp, Log, TEXT("Ability Input Pressed %s"), *InputTag.ToString());
	if (UDDSAbilitySystemComponent * ASC = GetDDSAbilitySystemComponent())
	{
		// MY_LOG(LogTemp, Log, TEXT("Ability Input Pressed %s"), *InputTag.ToString());
		ASC->AbilityInputTagPressed(InputTag);
	}
	else
	{
		MY_LOG(LogTemp, Warning, TEXT("DDSAbilitySystemComponent is nullptr"));
	}
			
}

void AInGamePlayerController::Input_AbilityInputReleased(FGameplayTag InputTag)
{
	if (bIsIgnoringGameInput)
		return;
	if (UDDSAbilitySystemComponent * ASC = GetDDSAbilitySystemComponent())
	{
		// MY_LOG(LogTemp, Log, TEXT("Ability Input Released %s"), *InputTag.ToString());
		ASC->AbilityInputTagReleased(InputTag);
	}
	else
	{
		MY_LOG(LogTemp, Warning, TEXT("DDSAbilitySystemComponent is nullptr"));
	}
}

void AInGamePlayerController::Input_Debug_PrintAttributes()
{
	if (UDDSAbilitySystemComponent * ASC = GetDDSAbilitySystemComponent())
	{
		const UDDSAttributeSet* AttributeSet = ASC->GetSet<UDDSAttributeSet>();
		if (AttributeSet)
		{
			AttributeSet->PrintAllAttributes();
		}
		else
		{
			MY_LOG(LogTemp, Warning, TEXT("AttributeSet is nullptr"));
		}
	}
	else
	{
		MY_LOG(LogTemp, Warning, TEXT("DDSAbilitySystemComponent is nullptr"));
	}
}


void AInGamePlayerController::SetIgnoreGameInput(bool bIgnore)
{
	if (bIgnore)
	{
		SetIgnoreMoveInput(true);
		SetIgnoreLookInput(true);
		bIsIgnoringGameInput = true;
	}
	else
	{
		SetIgnoreMoveInput(false);
		SetIgnoreLookInput(false);
		bIsIgnoringGameInput = false;
	}
}

APlayerBase* AInGamePlayerController::GetPlayerBase()
{
	if (!CachedPlayerBase.IsValid())
	{
		CachedPlayerBase = Cast<APlayerBase>(GetPawn());
	}
	return CachedPlayerBase.IsValid() ? CachedPlayerBase.Get() : nullptr;
}

UDDSAbilitySystemComponent* AInGamePlayerController::GetDDSAbilitySystemComponent()
{
	APlayerBase* PlayerBase = GetPlayerBase();
	if (!PlayerBase) return nullptr;
	return PlayerBase->GetDDSAbilitySystemComponent();
}

TArray<AActor*> AInGamePlayerController::GetFocusables() const
{
	TArray<AActor*> Focusables;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(),UFocusable::StaticClass(),Focusables);
	return Focusables;
}
 
// TArray<AActor*> AInGamePlayerController::GetFocusablesInSight() const
// {
// 	TArray<AActor*> Focusables = GetFocusables();
// 	TArray<AActor*> res;
// 	for (int i = Focusables.Num() - 1; i >= 0; --i)
// 	{
// 		AActor* Actor = Focusables[i];
// 		IFocusable* Focusable = Cast<IFocusable>(Actor);
// 		if (!Focusable) continue;
// 		if (Actor->WasRecentlyRendered(0.1f))
// 		{
// 			
// 			res.Add(Actor);
// 		}
// 	}
// 	return res;
// }

void AInGamePlayerController::UpdateMovementSpeedFromInput(const FVector2D& MoveVector)
{
	// L 스틱 입력의 크기(길이)에 따라 Normal / Slow 결정
	const float Magnitude = MoveVector.Size();

	// 임계값 예시: 0.7 이상은 "길게" (정확히는 강하게 기울인 상태), 그 이하는 "짧게"로 간주
	const float LongStickThreshold = 0.7f;
	const float DeadZone = 0.1f;

	if (Magnitude < DeadZone)
	{
		// 입력이 거의 없으면 기본 속도 유지
		CurrentMoveSpeedMode = AInGamePlayerController::EMoveSpeedMode::Normal;
		return;
	}

	if (Magnitude >= LongStickThreshold)
	{
		// L 스틱을 크게(길게) 기울임 -> 기본 속도 혹은 B와 함께면 스프린트
		if (bBPressed && BPressedTime >= BLongPressThreshold)
		{
			CurrentMoveSpeedMode = AInGamePlayerController::EMoveSpeedMode::Sprint;
		}
		else
		{
			CurrentMoveSpeedMode = AInGamePlayerController::EMoveSpeedMode::Normal;
		}
	}
	else
	{
		// L 스틱을 살짝(짧게) 기울임 -> 느린 속도
		CurrentMoveSpeedMode = AInGamePlayerController::EMoveSpeedMode::Slow;
	}
}

void AInGamePlayerController::ApplyMoveSpeedMode()
{
	ACharacter* MyCharacter = Cast<ACharacter>(GetPawn());
	if (!MyCharacter)
	{
		return;
	}
	UCharacterMovementComponent* MoveComp = MyCharacter->GetCharacterMovement();
	if (!MoveComp)
	{
		return;
	}

	switch (CurrentMoveSpeedMode)
	{
	case AInGamePlayerController::EMoveSpeedMode::Normal:
		MoveComp->MaxWalkSpeed = BaseMoveSpeed;
		break;
	case AInGamePlayerController::EMoveSpeedMode::Slow:
		MoveComp->MaxWalkSpeed = SlowMoveSpeed;
		break;
	case AInGamePlayerController::EMoveSpeedMode::Sprint:
		MoveComp->MaxWalkSpeed = BaseMoveSpeed * SprintMultiplier;
		break;
	default:
		break;
	}
}

void AInGamePlayerController::TryExecuteDodgeOrBackstep()
{
	APlayerBase* PlayerBase = GetPlayerBase();
	if (!PlayerBase)
	{
		return;
	}

	// 이동 입력이 있는지 여부
	const bool bHasMoveInput = !CachedMoveVector.IsNearlyZero(0.1f);

	// B 버튼이 짧게 눌렸다가 떼어졌다면 (press~release 사이 시간이 짧음)
	const bool bShortPress = (BPressedTime < BShortPressThreshold);
	const bool bLongPress = (BPressedTime >= BLongPressThreshold);
	
	UPlayerCombatComponent* CombatComp = PlayerBase->GetCombatComponent() ? Cast<UPlayerCombatComponent>(PlayerBase->GetCombatComponent()) : nullptr;
	if (!CombatComp)
	{
		return;
	}

	if (bHasMoveInput && !bLongPress)
	{
		// L스틱 + B 입력 -> 해당 방향 구르기
		CombatComp->TriggerDodge(CachedMoveVector);
	}
	else if (bShortPress)
	{
		// B 단독 짧은 입력 -> 백스텝
		CombatComp->TriggerBackstep();
	}
}
