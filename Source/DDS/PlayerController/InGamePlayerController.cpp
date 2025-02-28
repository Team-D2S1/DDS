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

#pragma region Legacy Input Binding
	// UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	// if(!EnhancedInputComponent) return;

	// EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	// EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::Attack);
	// EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ThisClass::Dash);
	// EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
#pragma endregion

	
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
	
	MyPawn->AddMovementInput(ForwardVector, MoveVector.Y);
	MyPawn->AddMovementInput(RightVector, MoveVector.X);
	// MyPawn->AddControllerYawInput(MoveVector.X);
	// MyPawn->AddControllerPitchInput(MoveVector.Y);
}

void AInGamePlayerController::Input_Jump()
{
	MY_LOG(LogTemp, Log, TEXT("Character Jump"));

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
	MY_LOG(LogTemp, Log, TEXT("Character Look"));
	
	FVector2D LookVector = Value.Get<FVector2D>();

	ACharacter* MyCharacter = GetCharacter();
	if(!MyCharacter) return;

	APlayerBase* MyPlayer = Cast<APlayerBase>(MyCharacter);
	if(!MyPlayer) return;

	FRotator CurrentRotation = MyPlayer->GetSpringArmComponent()->GetRelativeRotation();

	CurrentRotation.Yaw += LookVector.X;
	CurrentRotation.Pitch = CurrentRotation.Pitch + LookVector.Y;

	MyPlayer->GetSpringArmComponent()->SetRelativeRotation(CurrentRotation);
	
}
