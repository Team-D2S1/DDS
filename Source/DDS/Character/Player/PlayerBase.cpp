// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "PlayerBase.h"

#include "DDSPlayerState.h"
#include "Camera/CameraComponent.h"
#include "DDS/ETC/CustomLog.h"
#include "DDS/GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerBase::APlayerBase()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);


}

UAbilitySystemComponent* APlayerBase::GetAbilitySystemComponent() const
{
	return Super::GetAbilitySystemComponent();
}

void APlayerBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 서버에서 actor info 초기화
	InitAbilityActorInfo();
}

void APlayerBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 클라이언트에서 actor info 초기화
	InitAbilityActorInfo();
}

void APlayerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerBase::InitAbilityActorInfo()
{
	ADDSPlayerState* DDSPlayerState = GetPlayerState<ADDSPlayerState>();
	if (!DDSPlayerState)
	{
		MY_LOG(LogTemp, Error, TEXT("PlayerState is nullptr"));
		return;
	}
	DDSPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(DDSPlayerState, this);
	AbilitySystemComponent = DDSPlayerState->GetAbilitySystemComponent();
	AttributeSet = DDSPlayerState->GetAttributeSet();
}