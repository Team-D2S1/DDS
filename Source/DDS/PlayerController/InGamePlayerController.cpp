// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "InGamePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DDS/Character/CombatComponent/CombatComponent.h"
#include "DDS/ETC/CustomLog.h"

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

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(!Subsystem) return;

	Subsystem->AddMappingContext(InputMappingContext, 0);
}

void AInGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if(!EnhancedInputComponent) return;

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::Attack);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ThisClass::Dash);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

	MY_LOG(LogTemp, Log, TEXT("Setup Input Complete"));
}

void AInGamePlayerController::Move(const FInputActionValue& Value)
{
	MY_LOG(LogTemp, Log, TEXT("Character Move"));
}

void AInGamePlayerController::Jump()
{
	MY_LOG(LogTemp, Log, TEXT("Character Jump"));
}

void AInGamePlayerController::Attack()
{
	MY_LOG(LogTemp, Log, TEXT("Character Attack"));
}

void AInGamePlayerController::Dash()
{
	MY_LOG(LogTemp, Log, TEXT("Character Dash"));
}

void AInGamePlayerController::Look(const FInputActionValue& Value)
{
	MY_LOG(LogTemp, Log, TEXT("Character Look"));
}
