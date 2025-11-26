// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "PlayerBase.h"


#include "DDSGameplayTags.h"
#include "DDSPlayerState.h"
#include "Camera/CameraComponent.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "DataAsset/StartUpData/DataAsset_StartUpDataBase.h"
#include "DDS/ETC/CustomLog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Components/Inventory/InventoryComponent.h"
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameAbilitySystem/Abilities/DDSGameplayAbility.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/DDSHUD.h"

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
	CombatComponent->SetIsReplicated(true);
	// Owner 설정

	PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
	PlayerUIComponent->SetIsReplicated(true);
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
			LoadedData->GiveItemsToInventoryComponent(GetInventoryComponent());
		}
	}
	PlayerUIComponent->OnPawnInitializingFinished.Broadcast();
	// AbilitySystemComponent->AddReplicatedLooseGameplayTag(DDSGameplayTags::Shared_State_LockedOn);
}

void APlayerBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 클라이언트에서 actor info 초기화
	InitAbilityActorInfo();
	PlayerUIComponent->OnPawnInitializingFinished.Broadcast();
}

UPawnCombatComponent* APlayerBase::GetCombatComponent() const
{
	return CombatComponent;
}

UPawnUIComponent* APlayerBase::GetPawnUIComponent() const
{
	return PlayerUIComponent;
}

UPlayerUIComponent* APlayerBase::GetPlayerUIComponent() const
{
	return PlayerUIComponent;
}

UAISense_Sight::EVisibilityResult APlayerBase::CanBeSeenFrom(const FCanBeSeenFromContext& Context,
	FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested,
	float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate)
{
	// TODO
	// 만약 몬스터 뒤에 있고 수구리는 상태라면 -> Invisible (보이지 않음) 
	// Crouch 상태일 때 Tag 검색으로 판단하도록 하자
	
	return UAISense_Sight::EVisibilityResult::Visible;
}

UInventoryComponent* APlayerBase::GetInventoryComponent()
{
	if (CachedInventoryComponent.IsValid())
	{
		return CachedInventoryComponent.Get();
	}
	ADDSPlayerState* DDSPlayerState = GetPlayerState<ADDSPlayerState>();
	if (!DDSPlayerState)
	{
		MY_ERROR_DISPLAY_NET(HasAuthority(), TEXT("PlayerState is nullptr"));
		return nullptr;
	}
	CachedInventoryComponent = DDSPlayerState->GetInventoryComponent();
	if (!CachedInventoryComponent.IsValid())
	{
		MY_ERROR_DISPLAY_NET(HasAuthority(), TEXT("InventoryComponent is nullptr"));
		return nullptr;
	}
	return CachedInventoryComponent.Get();
}

void APlayerBase::OnPlayerDeath()
{
	if (DeathMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(DeathMontage, 1.0f);
            
			// 몽타주 길이 확인
			float MontageDuration = DeathMontage->GetPlayLength();
            
			// 몽타주 끝나기 직전에 일시정지
			FTimerHandle PauseHandle;
			GetWorld()->GetTimerManager().SetTimer(PauseHandle, [this, AnimInstance]()
			{
				// 마지막 포즈에서 멈춤
				AnimInstance->Montage_Pause(DeathMontage);
				UE_LOG(LogTemp, Warning, TEXT("Montage paused at end"));
			}, MontageDuration - 0.05f, false); // 끝나기 0.05초 전
		}
	}
}

void APlayerBase::OnPlayerRebirth()
{
	// 죽음 애니메이션 정지
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.f);
		GetMesh()->bPauseAnims = false;
	}

	// PlayerStart로 이동
	APlayerStart* PlayerStart = Cast<APlayerStart>(
		UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass())
	);

	if (PlayerStart)
	{
		SetActorLocation(PlayerStart->GetActorLocation());
		SetActorRotation(PlayerStart->GetActorRotation());
	}

	// 플레이어 체력 초기화
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(HealEffect, 1.f, EffectContext);

	if(SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}

	// 죽음 태그 제거
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead.Start"));
}

void APlayerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (CombatComponent)
	{
		AActor* Focused = FocusedObject;
		if(Focused)
		{
			// 캐릭터를 타겟 방향으로 회전 (Yaw만)
			FVector ToTarget = Focused->GetActorLocation() - GetActorLocation();
			FRotator LookRotation = ToTarget.Rotation();
			LookRotation.Pitch = 0.f;
			LookRotation.Roll = 0.f;
			
			SetActorRotation(LookRotation);
			
			// 컨트롤러도 타겟을 향하도록 (SpringArm이 따라감)
			if (Controller)
			{
				FRotator ControlRotation = LookRotation;
				ControlRotation.Pitch = -60.f; // 약간 위에서 내려다보는 각도
				Controller->SetControlRotation(ControlRotation);
			}
		}
	}
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerBase::OnDeathStartTagChanged(const FGameplayTag ChangedTag, int32 NumberOfTag)
{
	Super::OnDeathStartTagChanged(ChangedTag, NumberOfTag);
	
	if(HasAuthority())
	{
		AInGamePlayerController* PC = Cast<AInGamePlayerController>(GetController());
		if(PC)
		{
			PC->OnPlayerDeath();
		}
	}

	OnPlayerDeath();
}

void APlayerBase::OnDeathEndTagChanged(const FGameplayTag ChangedTag, int32 NumberOfTag)
{
	Super::OnDeathEndTagChanged(ChangedTag, NumberOfTag);
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
	
	// 로컬에서도 즉시 복구 (서버에서 실행)
	if (HasAuthority())
	{
		OnRep_FocusedObject();
	}
}

void APlayerBase::OnRep_FocusedObject()
{
	Super::OnRep_FocusedObject();
	
	// 포커스 해제 시 SpringArm과 카메라를 정상 상태로 복구
	if (!FocusedObject)
	{
		if (SpringArm)
		{
			SpringArm->bUsePawnControlRotation = true;
		}
		
		if (Camera)
		{
			Camera->bUsePawnControlRotation = false;
		}
		
		// Controller rotation도 초기화
		if (Controller)
		{
			// 현재 캐릭터가 바라보는 방향으로 컨트롤러 회전 복구
			FRotator CurrentRotation = GetActorRotation();
			CurrentRotation.Pitch = -30.f; // 기본 카메라 피치
			Controller->SetControlRotation(CurrentRotation);
		}
	}
	else
	{
		// 포커스 시작 시
		if (SpringArm)
		{
			SpringArm->bUsePawnControlRotation = true;
		}
	}
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
	CachedInventoryComponent = TWeakObjectPtr<UInventoryComponent>(DDSPlayerState->GetInventoryComponent());
	PlayerUIComponent->BroadcastInitialValues(AttributeSet);
	AbilitySystemComponent->BindAttributeValueChangeDelegates(AttributeSet);

	// PlayerUIComponent가 ASC의 경험치/레벨업 알림을 구독하도록 설정
	PlayerUIComponent->BindExperienceNotifications(AbilitySystemComponent);
}

