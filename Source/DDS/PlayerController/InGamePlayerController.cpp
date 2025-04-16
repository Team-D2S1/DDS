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

	UDDSInputComponent* DDSInputComponent = CastChecked<UDDSInputComponent>(InputComponent);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Jump, ETriggerEvent::Started, this, &ThisClass::Input_Jump);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	DDSInputComponent->BindNativeAction(InputConfigDataAsset, DDSGameplayTags::InputTag_LockOn, ETriggerEvent::Started, this, &ThisClass::Input_LockOn);

	DDSInputComponent->BindAbilityInputAction(InputConfigDataAsset,this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
	MY_LOG(LogTemp, Log, TEXT("Setup Input Complete"));
}

void AInGamePlayerController::Input_Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();
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

void AInGamePlayerController::Input_Jump()
{
	ACharacter* MyCharacter = GetCharacter();

	if(!MyCharacter) return;

	MyCharacter->Jump();
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

void AInGamePlayerController::Input_ShowCrafting()
{
	ADDSHUD* HUD = Cast<ADDSHUD>(GetHUD());
	if (!HUD) return;
	
}

void AInGamePlayerController::Input_AbilityInputPressed(FGameplayTag InputTag)
{
	MY_CLOG_DISPLAY_NET(FColor::Cyan,HasAuthority(), TEXT("Ability Input Pressed %s"), *InputTag.ToString());
	GetDDSAbilitySystemComponent()->AbilityInputTagPressed(InputTag);
			
}

void AInGamePlayerController::Input_AbilityInputReleased(FGameplayTag InputTag)
{
	GetDDSAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
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
