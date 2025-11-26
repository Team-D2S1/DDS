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
#include "GameMode/DDSGameMode.h"
#include "Interfaces/Focusable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/HUD/DDSHUD.h"
#include "UI/HUD/GameOverUI.h"

AInGamePlayerController::AInGamePlayerController()
{
	// CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));

	
}


void AInGamePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bBPressed)
	{
		BPressedTime += DeltaSeconds;
	}

	// 태그 관리 로직
	if (UDDSAbilitySystemComponent* ASC = GetDDSAbilitySystemComponent())
	{
		APawn* MyPawn = GetPawn();
		if (MyPawn)
		{
			const FVector Velocity = MyPawn->GetVelocity();
			const float Speed = Velocity.Size2D();
          
			// 움직임 여부 판단 
			const bool bIsActuallyMoving = Speed > 3.0f; 
			
			const bool bIsSprinting = bIsActuallyMoving && (CurrentMoveSpeedMode == EMoveSpeedMode::Sprint);


			// Moving 태그 처리
			const bool bHasMovingTag = ASC->HasMatchingGameplayTag(DDSGameplayTags::Player_State_Moving);
			if (bIsActuallyMoving && !bHasMovingTag)
			{
				ASC->Multicast_AddLooseGameplayTag(DDSGameplayTags::Player_State_Moving);
			}
			else if (!bIsActuallyMoving && bHasMovingTag)
			{
				ASC->Multicast_RemoveLooseGameplayTag(DDSGameplayTags::Player_State_Moving);
			}

			// Sprinting 태그 처리
			const bool bHasSprintTag = ASC->HasMatchingGameplayTag(DDSGameplayTags::Player_State_Sprinting);
          
			if (bIsSprinting && !bHasSprintTag)
			{
				ASC->Multicast_AddLooseGameplayTag(DDSGameplayTags::Player_State_Sprinting);
			}
			else if (!bIsSprinting && bHasSprintTag)
			{
				ASC->Multicast_RemoveLooseGameplayTag(DDSGameplayTags::Player_State_Sprinting);
			}
		}
	}
}

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	Super::BeginPlay();


	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputConfigDataAsset)
			{
				Subsystem->ClearAllMappings(); 
				Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
				Subsystem->AddMappingContext(InputConfigDataAsset->UIInputMappingContext, 1);
			}
		}
	}
	
	FInputModeGameOnly GameInputMode;
	SetInputMode(GameInputMode);
	

	
	
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
	if(!Subsystem)
	{
		MY_LOG(LogTemp, Error, TEXT("Enhanced Input Subsystem is null, cannot setup input"));
		return;
	}

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
	
	// 키보드 Sprint (Shift) 입력 바인딩
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Sprint, ETriggerEvent::Started, this, &ThisClass::Input_Sprint_Pressed);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Sprint, ETriggerEvent::Completed, this, &ThisClass::Input_Sprint_Released);
	
	// 키보드 Walk (Ctrl) 입력 바인딩
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Walk, ETriggerEvent::Started, this, &ThisClass::Input_Walk_Pressed);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Walk, ETriggerEvent::Completed, this, &ThisClass::Input_Walk_Released);
	
 	DDSInputComponent->BindUIActions(InputConfigDataAsset, this, &ThisClass::Input_UIInputPressed, &ThisClass::Input_UIInputReleased);
	DDSInputComponent->BindAbilityInputActions(InputConfigDataAsset,this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);


	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Debug_PrintAttributes, ETriggerEvent::Started, this, &ThisClass::Input_Debug_PrintAttributes);

	// Cheat Key Bindings (
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Cheat_AddExp, ETriggerEvent::Started, this, &ThisClass::Cheat_AddExp);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Cheat_AddAttributePoints, ETriggerEvent::Started, this, &ThisClass::Cheat_AddAttributePoints);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Cheat_LevelUp, ETriggerEvent::Started, this, &ThisClass::Cheat_LevelUp);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Cheat_FullHeal, ETriggerEvent::Started, this, &ThisClass::Cheat_FullHeal);
	
	MY_LOG(LogTemp, Log, TEXT("Setup Input Complete"));
}

void AInGamePlayerController::OnPlayerDeath()
{
	// 플레이어 조작 불가능하게
	DisableInput(this);
	
	// 게임오버 표시
	ADDSHUD* Ingame_HUD = Cast<ADDSHUD>(GetHUD());
	if(Ingame_HUD)
	{
		Ingame_HUD->ShowGameOverWidget();
	}
}

void AInGamePlayerController::OnPlayerDeathEnd()
{
	EnableInput(this);
	
	// 플레이어 부활처리
	GetPlayerBase()->OnPlayerRebirth();

	// 플레이어에 붙어있는 사망 태그 떼어주기
	

	// 몬스터 리셋
	if(ADDSGameMode* GameMode = Cast<ADDSGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->ResetAllMonsters();
	}
}

void AInGamePlayerController::Input_Move(const FInputActionValue& Value)
{
	// MY_LOG_DISPLAY("Move %s", *Value.ToString());
	FVector2D MoveVector = Value.Get<FVector2D>();
	//Move 태그가 차단되어있으면 이동 무시
	UDDSAbilitySystemComponent* ASC = GetDDSAbilitySystemComponent();
	if (ASC && ASC->HasMatchingGameplayTag(DDSGameplayTags::Player_State_BlockMove))
	{
		return;
	}

	UpdateMovementSpeedFromInput(MoveVector);
	ApplyMoveSpeedMode();

	FRotator YawRotator(0.f, GetControlRotation().Yaw, 0.f);
	FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	// 실제 이동방향으로 캐시
	FVector WorldMoveDirection = ForwardVector * MoveVector.Y + RightVector * MoveVector.X;
	CachedMoveVector = WorldMoveDirection.GetSafeNormal2D();
	
	APawn* MyPawn = GetPawn();
	if(!MyPawn) return;
	// Dodge태그시 이동 무시
	if (ASC && ASC->HasMatchingGameplayTag(DDSGameplayTags::Player_Ability_Dodge))
	{
		return;
	}


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
	
	TryExecuteDodgeOrBackstep();
	bBPressed = false;
	BPressedTime = 0.f;
	
}

void AInGamePlayerController::Input_Sprint_Pressed(const FInputActionValue& Value)
{
	bSprintKeyPressed = true;

	// Sprint 태그 추가 (Multicast로 모든 클라이언트에 복제)
	if (UDDSAbilitySystemComponent* ASC = GetDDSAbilitySystemComponent())
	{
		ASC->Multicast_AddLooseGameplayTag(DDSGameplayTags::Player_State_Sprinting);
	}
}

void AInGamePlayerController::Input_Sprint_Released(const FInputActionValue& Value)
{
	bSprintKeyPressed = false;
	
	// Sprint 모드였다면 Normal로 복귀
	if (CurrentMoveSpeedMode == EMoveSpeedMode::Sprint)
	{
		CurrentMoveSpeedMode = EMoveSpeedMode::Normal;
		ApplyMoveSpeedMode();
	}

	// Sprint 태그 제거 (Multicast로 모든 클라이언트에 복제)
	if (UDDSAbilitySystemComponent* ASC = GetDDSAbilitySystemComponent())
	{
		ASC->Multicast_RemoveLooseGameplayTag(DDSGameplayTags::Player_State_Sprinting);
	}
}

void AInGamePlayerController::Input_Walk_Pressed(const FInputActionValue& Value)
{
	bWalkKeyPressed = true;

	// Walk 태그 추가 (Multicast로 모든 클라이언트에 복제)
	if (UDDSAbilitySystemComponent* ASC = GetDDSAbilitySystemComponent())
	{
		ASC->Multicast_AddLooseGameplayTag(DDSGameplayTags::Player_State_Walking);
	}
}

void AInGamePlayerController::Input_Walk_Released(const FInputActionValue& Value)
{
	bWalkKeyPressed = false;
	
	// Slow 모드였다면 Normal로 복귀
	if (CurrentMoveSpeedMode == EMoveSpeedMode::Slow)
	{
		CurrentMoveSpeedMode = EMoveSpeedMode::Normal;
		ApplyMoveSpeedMode();
	}

	// Walk 태그 제거 (Multicast로 모든 클라이언트에 복제)
	if (UDDSAbilitySystemComponent* ASC = GetDDSAbilitySystemComponent())
	{
		ASC->Multicast_RemoveLooseGameplayTag(DDSGameplayTags::Player_State_Walking);
	}
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

	// MY_CLOG_DISPLAY_NET(FColor::White, HasAuthority(), TEXT("Start Lock On"));
	// 대상을 찾는다.
	// 대상은 현재 플레이어 카메라 기준 가장 가운데에 가까운 적
	// 대상이 없으면? 그냥 취소
	// UCameraComponent* Cam = PlayerBase->GetCameraComponent();
	// UPlayerCombatComponent* CombatComponent = PlayerBase->GetCombatComponent();
	AActor* target = nullptr;

	int32 SizeX, SizeY;
	GetViewportSize(SizeX, SizeY);
	FVector2D ScreenCenter(SizeX * 0.5f, SizeY * 0.5f); // 실제 화면의 중앙 픽셀 좌표
	float MinDistanceFromCenter = std::numeric_limits<float>::max();
	
	if (!focusedObject)
	{
		for (auto FocusableActor : GetFocusables())
		{
			if (!FocusableActor) continue;

			// 월드 좌표를 화면 좌표로 변환
			FVector2D ScreenLocation;
			bool bIsOnScreen = ProjectWorldLocationToScreen(FocusableActor->GetActorLocation(), ScreenLocation);

			if (bIsOnScreen)
			{
				// 화면 중앙과의 거리 계산 (Squared를 쓰면 Sqrt 연산을 줄여 최적화 가능)
				float DistSquared = FVector2D::DistSquared(ScreenLocation, ScreenCenter);
				
				if (DistSquared < MinDistanceFromCenter)
				{
					// 레이캐스트
					FHitResult HitResult;
					FVector StartLocation = PlayerBase->GetCameraComponent()->GetComponentLocation();
					FVector EndLocation = FocusableActor->GetActorLocation();
                
					FCollisionQueryParams QueryParams;
					QueryParams.AddIgnoredActor(PlayerBase);

					// 벽 등에 막히는지 확인
					bool bHit = GetWorld()->LineTraceSingleByChannel(
						HitResult,
						StartLocation,
						EndLocation,
						ECC_Visibility,
						QueryParams
					);

					// 레이가 무언가에 맞았고, 그 맞은 대상이 우리가 보고 있는 적이라면 (즉, 벽이 없다면)
					if (bHit && HitResult.GetActor() == FocusableActor)
					{
						MinDistanceFromCenter = DistSquared;
						target = FocusableActor;
					}
				}
			}
		}
	}
	IFocusable* targetFocusable = Cast<IFocusable>(target);
	
	if (target)
	{
		// MY_CLOG_DISPLAY_NET(FColor::White, HasAuthority(), TEXT("Lock On Target : %s"), *target->GetName());
  
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

void AInGamePlayerController::Cheat_AddExp()
{
	if (!HasAuthority())
	{
		MY_LOG_DISPLAY("Cheat functions can only be called on the server");
		return;
	}

	UDDSAbilitySystemComponent* ASC = GetDDSAbilitySystemComponent();
	if (!ASC)
	{
		MY_LOG_DISPLAY("ASC not found");
		return;
	}

	const UAttributeSet* AttributeSetConst = ASC->GetAttributeSet(UDDSAttributeSet::StaticClass());

	float Amount = 500.f;

	UDDSAttributeSet* AS = const_cast<UDDSAttributeSet*>(Cast<UDDSAttributeSet>(AttributeSetConst));
	if (!AS)
	{
		MY_LOG_DISPLAY("AttributeSet not found");
		return;
	}
	const float OldEnergy = AS->GetEnergy();
	

	ASC->AddExperienceAndCheckLevelUp(Amount);
	

	

	MY_CLOG_DISPLAY_NET(FColor::Cyan, true, 
		TEXT("💎 Cheat: Added %.0f Energy (%.0f -> %.0f)"), 
		Amount, OldEnergy, AS->GetEnergy());
}

void AInGamePlayerController::Cheat_AddAttributePoints()
{
	if (!HasAuthority())
	{
		MY_LOG_DISPLAY("Cheat functions can only be called on the server");
		return;
	}

	UDDSAbilitySystemComponent* ASC = GetDDSAbilitySystemComponent();
	if (!ASC)
	{
		MY_LOG_DISPLAY("ASC not found");
		return;
	}

	const UAttributeSet* AttributeSetConst = ASC->GetAttributeSet(UDDSAttributeSet::StaticClass());
	UDDSAttributeSet* AS = const_cast<UDDSAttributeSet*>(Cast<UDDSAttributeSet>(AttributeSetConst));
	if (!AS)
	{
		MY_LOG_DISPLAY("AttributeSet not found");
		return;
	}

	const float Amount = 10.f;
	const float OldPoints = AS->GetAttributePoints();
	AS->SetAttributePoints(OldPoints + Amount);

	MY_CLOG_DISPLAY_NET(FColor::Green, true, 
		TEXT("⭐ Cheat: Added %.0f AttributePoints (%.0f -> %.0f)"), 
		Amount, OldPoints, AS->GetAttributePoints());
}

void AInGamePlayerController::Cheat_LevelUp()
{
	if (!HasAuthority())
	{
		MY_LOG_DISPLAY("Cheat functions can only be called on the server");
		return;
	}

	UDDSAbilitySystemComponent* ASC = GetDDSAbilitySystemComponent();
	if (!ASC)
	{
		MY_LOG_DISPLAY("ASC not found");
		return;
	}

	ASC->LevelUp(1);
}

void AInGamePlayerController::Cheat_FullHeal()
{
	if (!HasAuthority())
	{
		MY_LOG_DISPLAY("Cheat functions can only be called on the server");
		return;
	}

	UDDSAbilitySystemComponent* ASC = GetDDSAbilitySystemComponent();
	if (!ASC)
	{
		MY_LOG_DISPLAY("ASC not found");
		return;
	}

	const UAttributeSet* AttributeSetConst = ASC->GetAttributeSet(UDDSAttributeSet::StaticClass());
	UDDSAttributeSet* AS = const_cast<UDDSAttributeSet*>(Cast<UDDSAttributeSet>(AttributeSetConst));
	if (!AS)
	{
		MY_LOG_DISPLAY("AttributeSet not found");
		return;
	}

	AS->SetHealth(AS->GetHealthMax());
	AS->SetStamina(AS->GetStaminaMax());
	AS->SetMana(AS->GetManaMax());

	MY_CLOG_DISPLAY_NET(FColor::Green, true, TEXT("💚 Cheat: Full Heal!"));
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
    const float Magnitude = MoveVector.Size();

    const float FastWalkThreshold = 0.7f;
    const float DeadZone = 0.1f;
    const float DirectionThreshold = 0.8f; // 대략 36도 이내

    APawn* CurrentPawn = GetPawn();
    if (!CurrentPawn) return;

    FRotator ControlRot = GetControlRotation();
    FRotator YawRotation(0, ControlRot.Yaw, 0);
    
    FVector CameraForward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    FVector CameraRight   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    FVector DesiredDir = (CameraForward * MoveVector.Y + CameraRight * MoveVector.X);
    DesiredDir.Normalize();

    FVector ActorForward = CurrentPawn->GetActorForwardVector();
    float DirectionDot   = FVector::DotProduct(DesiredDir, ActorForward);

    const bool bIsMovingForward = (Magnitude > DeadZone) && (DirectionDot >= DirectionThreshold);

    // --- 스태미나 체크 추가 부분 ---
    bool bHasAnyStamina = false;
    bool bStaminaEnoughForSprint = false;

    if (UDDSAbilitySystemComponent* ASC = GetDDSAbilitySystemComponent())
    {
        const UAttributeSet* AttributeSetConst = ASC->GetAttributeSet(UDDSAttributeSet::StaticClass());
        const UDDSAttributeSet* AS = Cast<UDDSAttributeSet>(AttributeSetConst);
        if (AS)
        {
            const float CurrentStamina = AS->GetStamina();

            // 0 이하면 스태미나 없음
            bHasAnyStamina = CurrentStamina > 0.f;

            // 10 이상이어야 Sprint 가능
            bStaminaEnoughForSprint = CurrentStamina >= 10.f;

            // 스태미나가 0 이 되었으면 스프린트 강제 해제
            if (!bHasAnyStamina && CurrentMoveSpeedMode == EMoveSpeedMode::Sprint)
            {
                CurrentMoveSpeedMode = EMoveSpeedMode::Normal;
                ApplyMoveSpeedMode();
            }
        }
    }


    if (Magnitude < DeadZone)
    {
        if (CurrentMoveSpeedMode == EMoveSpeedMode::Sprint)
        {
            CurrentMoveSpeedMode = EMoveSpeedMode::Normal;
        }
        return;
    }

    // 1순위: Walk 키
    if (bWalkKeyPressed)
    {
        CurrentMoveSpeedMode = EMoveSpeedMode::Slow;
        return;
    }

    // 2순위: Shift 스프린트 (전방 + 스태미나 충분)
    if (bSprintKeyPressed && bIsMovingForward && bStaminaEnoughForSprint)
    {
        CurrentMoveSpeedMode = EMoveSpeedMode::Sprint;
        return;
    }

    // 3순위: B 길게 누르기 스프린트 (패드)
    if (bBPressed && BPressedTime >= BLongPressThreshold && bStaminaEnoughForSprint && bIsMovingForward && Magnitude >= FastWalkThreshold)
    {
        CurrentMoveSpeedMode = EMoveSpeedMode::Sprint;
    }
    else if (Magnitude >= FastWalkThreshold)
    {
        CurrentMoveSpeedMode = EMoveSpeedMode::Normal;
    }
    else
    {
        CurrentMoveSpeedMode = EMoveSpeedMode::Slow;
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
	UDDSAbilitySystemComponent * ASC = GetDDSAbilitySystemComponent();
	switch (CurrentMoveSpeedMode)
	{
	case AInGamePlayerController::EMoveSpeedMode::Normal:
		MoveComp->MaxWalkSpeed = BaseMoveSpeed;
		break;
	case AInGamePlayerController::EMoveSpeedMode::Slow:
		MoveComp->MaxWalkSpeed = SlowMoveSpeed;
		break;
	case AInGamePlayerController::EMoveSpeedMode::Sprint:
		MoveComp->MaxWalkSpeed = SprintMoveSpeed;
		
		ASC->Server_StopStaminaRegen();
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
	const bool bShortPress = (BPressedTime <= BShortPressThreshold);
	const bool bLongPress = (BPressedTime > BShortPressThreshold && BPressedTime <= BLongPressThreshold);
	
	UPlayerCombatComponent* CombatComp = PlayerBase->GetCombatComponent() ? Cast<UPlayerCombatComponent>(PlayerBase->GetCombatComponent()) : nullptr;
	if (!CombatComp)
	{
		return;
	}
	if (!bHasMoveInput)
	{
		return;
	}
	

	if (bShortPress)
	{
		// 짧게 누름 -> Backstep
		CombatComp->Server_TriggerBackstep();
	}
	else if (bLongPress)
	{
		// 중간 길이로 누름 -> Dodge (Sprint 시간보다는 짧게)
		CombatComp->Server_TriggerDodge(CachedMoveVector);
	}
	// Sprint 시간 이상 누른 경우는 여기서 처리하지 않음 (Input_B_Released에서 이미 필터링됨)
}
