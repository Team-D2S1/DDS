// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "InGamePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DDS/Character/CombatComponent/CombatComponent.h"
#include "DDS/Character/Player/PlayerBase.h"
#include "DDS/DataAsset/Input/DataAsset_InputConfig.h"
#include "DDS/ETC/CustomLog.h"
#include "DDS/DDSGameplayTags.h"
#include "DDS/Components/Input/DDSInputComponent.h"
#include "GameFramework/SpringArmComponent.h"

AInGamePlayerController::AInGamePlayerController()
{
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
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
	DDSInputComponent->BindAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	DDSInputComponent->BindAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Jump, ETriggerEvent::Started, this, &ThisClass::Input_Jump);
	DDSInputComponent->BindAction(InputConfigDataAsset, DDSGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

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

// void AInGamePlayerController::Attack()
// {
// 	MY_LOG(LogTemp, Log, TEXT("Character Attack"));
// }
//
// void AInGamePlayerController::Dash()
// {
// 	MY_LOG(LogTemp, Log, TEXT("Character Dash"));
// }

void AInGamePlayerController::Input_Look(const FInputActionValue& Value)
{
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
