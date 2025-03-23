// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "PlayerBase.h"

#include "DDSGameplayTags.h"
#include "DDSPlayerState.h"
#include "Camera/CameraComponent.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "DataAsset/StartUpData/DataAsset_StartUpDataBase.h"
#include "DDS/ETC/CustomLog.h"
#include "DDS/GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerBase::APlayerBase()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	// GetCharacterMovement()->bUseControllerDesiredRotation = false; // 강의엔 없는데 넣어봄
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);


	CombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	// CombatComponent->SetIsReplicated(true);
	// Owner 설정
}

// UAbilitySystemComponent* APlayerBase::GetAbilitySystemComponent() const
// {
// 	return Super::GetAbilitySystemComponent();
// }

void APlayerBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	
	// 서버에서 actor info 초기화
	InitAbilityActorInfo();
	if (!EntityStartUpDataBase.IsNull())
	{
		MY_LOG(LogTemp,Log,TEXT("EntityStartUpDataBase not null"))
		// 플레이어는 그 수가 예측가능하므로, 동기 로딩 사용
		UDataAsset_StartUpDataBase* LoadedData = EntityStartUpDataBase.LoadSynchronous();
		if (LoadedData)
		{
			MY_LOG(LogTemp,Log,TEXT("LoadedData not null"))
			LoadedData->GiveToAbilitySystemComponent(AbilitySystemComponent);
		}
	}
	// AbilitySystemComponent->AddReplicatedLooseGameplayTag(DDSGameplayTags::Shared_State_LockedOn);
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
	
	if (CombatComponent)
	{
		AActor* Focused = FocusedObject;
		if(Focused)
		   {
		   	FVector ToTarget = Focused->GetActorLocation() - GetActorLocation();
		   	FRotator LookRotation = ToTarget.Rotation();

			
			SetActorRotation(LookRotation);
			
		   	// 카메라 회전 (SpringArm이 따라감)
		   	if (Controller)
		   	{
		   		LookRotation.Pitch = -30.f;
		   		Controller->SetControlRotation(LookRotation);
		   	}
			if (Camera){
			 	FRotator CamLookRotation = (Focused->GetActorLocation() - Camera->GetComponentLocation()).Rotation();
				Camera -> SetWorldRotation(CamLookRotation);
			}
		}
	}
	// if (GetDDSAbilitySystemComponent()->HasMatchingGameplayTag(DDSGameplayTags::Shared_State_LockedOn))
	// {
	// 	//이름출력
	// 	DEBUG_CLOG_DISPLAY_NET(FColor::Yellow, HasAuthority(),TEXT("%s is locked on"), *GetName());
	// }
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerBase::Server_SetFocusedObject(AActor* InFocusedObject)
{
	Super::Server_SetFocusedObject(InFocusedObject);
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void APlayerBase::Server_ClearFocusedObject()
{
	Super::Server_ClearFocusedObject();
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void APlayerBase::InitAbilityActorInfo()
{
	ADDSPlayerState* DDSPlayerState = GetPlayerState<ADDSPlayerState>();
	// MY_LOG(LogTemp,Log,TEXT("InitAbilityActorInfo"))
	if (!DDSPlayerState)
	{
		MY_LOG(LogTemp, Error, TEXT("PlayerState is nullptr"));
		return;
	}
	DDSPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(DDSPlayerState, this);
	AbilitySystemComponent = DDSPlayerState->GetDDSAbilitySystemComponent();
	AttributeSet = DDSPlayerState->GetDDSAttribueSet();

	

}

