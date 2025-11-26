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
	SpringArm->SetIsReplicated(false);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->SetIsReplicated(false);

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
			// 캐릭터 회전 (Yaw): 타겟을 향해 부드럽게 회전
			FVector ToTarget = Focused->GetActorLocation() - GetActorLocation();
			FRotator LookRotation = ToTarget.Rotation();
			LookRotation.Pitch = 0.f;
			LookRotation.Roll = 0.f;
          
			// FMath::RInterpTo를 사용하여 현재 회전에서 목표 회전까지 부드럽게 이동 (속도 15.0f)
			FRotator SmoothActorRot = FMath::RInterpTo(GetActorRotation(), LookRotation, DeltaSeconds, 15.0f);
			SetActorRotation(SmoothActorRot);
          
			// 컨트롤러(SpringArm) 회전 약간 위에서 내려다보는 각도로 부드럽게 이동
			if (Controller)
			{
				FRotator TargetControlRotation = LookRotation;
				TargetControlRotation.Pitch = -30.f; // 목표 피치 각도
             
				FRotator SmoothControlRot = FMath::RInterpTo(Controller->GetControlRotation(), TargetControlRotation, DeltaSeconds, 10.0f);
				Controller->SetControlRotation(SmoothControlRot);
			}

			// 카메라 자체 보정 타겟을 정중앙에 두기 위한 미세 조정
			if (Camera)
			{
				Camera->bUsePawnControlRotation = false;

				FVector CameraToTarget = Focused->GetActorLocation() - Camera->GetComponentLocation();
				FRotator CameraLookRotation = CameraToTarget.Rotation();
				CameraLookRotation.Pitch = FMath::Clamp(CameraLookRotation.Pitch, -45.f, 45.f);
				CameraLookRotation.Roll = 0.f;

				// 카메라도 RInterpTo로 부드럽게 목표 지점을 바라보게 함 (속도 15.0f)
				FRotator SmoothCameraRot = FMath::RInterpTo(Camera->GetComponentRotation(), CameraLookRotation, DeltaSeconds, 15.0f);
				Camera->SetWorldRotation(SmoothCameraRot);  
			}
		}else if (bIsResettingCamera && Camera)
		{
			// 카메라 컴포넌트 자체 정렬 (SpringArm과 나란하게)
			FRotator CurrentRelRot = Camera->GetRelativeRotation();
			if (!CurrentRelRot.IsNearlyZero(0.1f))
			{
				FRotator SmoothRelRot = FMath::RInterpTo(CurrentRelRot, FRotator::ZeroRotator, DeltaSeconds, 5.0f);
				Camera->SetRelativeRotation(SmoothRelRot);
			}

			// 스프링암(컨트롤러) 각도 복구 (Pitch만 -30도로, Yaw는 유지)
			FRotator CurrentControlRot = Controller->GetControlRotation();
        
			// 목표 각도 설정
			// Pitch: -30도 (기본 쿼터뷰 각도)
			// Yaw: 현재 보고 있는 방향 유지
			FRotator TargetControlRot = CurrentControlRot; 
			TargetControlRot.Pitch = -30.0f; 
			TargetControlRot.Roll = 0.0f;
			if (FMath::IsNearlyEqual(CurrentControlRot.Pitch, -30.0f, 0.5f) && CurrentRelRot.IsNearlyZero(0.1f))
			{
				bIsResettingCamera = false;
			}
			else
			{
				// Pitch만 부드럽게 변경 (Yaw는 현재 값 그대로 유지되므로 화면이 안 돌아감)
				FRotator SmoothControlRot = FMath::RInterpTo(CurrentControlRot, TargetControlRot, DeltaSeconds, 5.0f);
				Controller->SetControlRotation(SmoothControlRot);
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
			if(UAbilitySystemComponent* ASC = PC->GetPlayerBase()->GetAbilitySystemComponent())
			{
				FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag("State.Dead.Start");
				if(!ASC->HasMatchingGameplayTag(DeathTag)) // 부활일 경우 무시
				{
					return;
				}
			}
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
    
	if (!FocusedObject)
	{
		// 현재 카메라가 보고 있는 방향을 컨트롤러에게 전달 (화면 튐 방지)
		if (Controller && Camera)
		{
			FRotator CurrentCameraRot = Camera->GetComponentRotation();
			Controller->SetControlRotation(CurrentCameraRot);
		}

		if (SpringArm)
		{
			SpringArm->bUsePawnControlRotation = true;
		}
       
		if (Camera)
		{
			Camera->bUsePawnControlRotation = false;  
			bIsResettingCamera = true; 
		}
	}
	else
	{
		// 락온 시작 시 복구 모드 즉시 중단
		bIsResettingCamera = false;

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

