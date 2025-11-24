// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "EntityBase.h"

#include "DDSGameplayTags.h"
#include "DDSTypes/DDSEnumTypes.h"
#include "ETC/CustomLog.h"
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameAbilitySystem/DDSAttributeSet.h"
#include "GameAbilitySystem/DDSPlayerAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

AEntityBase::AEntityBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEntityBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// if (EntityStartUpDataBase.IsNull())
	// {
	// 	MY_LOG(LogTemp,Error,TEXT("Forgot to assign start up data to %s"),*GetName())
	// }
	if (!ensure(!EntityStartUpDataBase.IsNull()))
	{
		MY_LOG(LogTemp,Error,TEXT("Forgot to assign start up data to %s"),*GetName())
	}
}

UAbilitySystemComponent* AEntityBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPawnCombatComponent* AEntityBase::GetCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AEntityBase::GetPawnUIComponent() const
{
	return nullptr;
}

UAttributeSet* AEntityBase::GetAttributeSet() const
{
	return AttributeSet;
}

UDDSPlayerAttributeSet* AEntityBase::GetPlayerAttributeSet() const
{
	return Cast<UDDSPlayerAttributeSet>(AttributeSet);
}

void AEntityBase::Multicast_LinkAnimLayer_Implementation(TSubclassOf<UAnimInstance> InAnimLayerClass)
{
	// 데디케이티드 서버에서는 애니메이션 처리를 스킵
	if (GetNetMode() == NM_DedicatedServer)
	{
		MY_LOG(LogTemp, Log, TEXT("Skipping LinkAnimLayer on dedicated server"));
		return;
	}

	bool bIsServer = HasAuthority();
	ENetRole localRole = GetLocalRole();
	ENetRole remoteRole = GetRemoteRole();
	
	MY_LOG(LogTemp, Warning, TEXT("[%s] Multicast_LinkAnimLayer called - Role: %d, RemoteRole: %d, AnimLayer: %s"), 
		bIsServer ? TEXT("Server") : TEXT("Client"),
		static_cast<int32>(localRole),
		static_cast<int32>(remoteRole),
		InAnimLayerClass ? *InAnimLayerClass->GetName() : TEXT("nullptr"));
	
	if (!InAnimLayerClass)
	{
		MY_LOG(LogTemp, Error, TEXT("InAnimLayerClass is nullptr"));
		return;
	}
	
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if (SkeletalMesh)
	{
		UAnimInstance* CurrentAnimInstance = SkeletalMesh->GetAnimInstance();
		MY_LOG(LogTemp, Log, TEXT("Current AnimInstance: %s"), CurrentAnimInstance ? *CurrentAnimInstance->GetName() : TEXT("nullptr"));
		
		SkeletalMesh->LinkAnimClassLayers(InAnimLayerClass);
		
		MY_LOG(LogTemp, Warning, TEXT("[%s] Successfully linked anim layer: %s"), 
			bIsServer ? TEXT("Server") : TEXT("Client"),
			*InAnimLayerClass->GetName());
	}
	else
	{
		MY_LOG(LogTemp,Error,TEXT("MeshComp is nullptr"));
	}
}

void AEntityBase::Multicast_UnlinkAnimLayer_Implementation(TSubclassOf<UAnimInstance> InAnimLayerClass)
{
	// 데디케이티드 서버에서는 애니메이션 처리를 스킵
	if (GetNetMode() == NM_DedicatedServer)
	{
		MY_LOG(LogTemp, Log, TEXT("Skipping UnlinkAnimLayer on dedicated server"));
		return;
	}

	bool bIsServer = HasAuthority();
	ENetRole localRole = GetLocalRole();
	ENetRole remoteRole = GetRemoteRole();
	
	MY_LOG(LogTemp, Warning, TEXT("[%s] Multicast_UnlinkAnimLayer called - Role: %d, RemoteRole: %d, AnimLayer: %s"), 
		bIsServer ? TEXT("Server") : TEXT("Client"),
		static_cast<int32>(localRole),
		static_cast<int32>(remoteRole),
		InAnimLayerClass ? *InAnimLayerClass->GetName() : TEXT("nullptr"));
	
	if (!InAnimLayerClass)
	{
		MY_LOG(LogTemp, Error, TEXT("InAnimLayerClass is nullptr"));
		return;
	}
	
	USkeletalMeshComponent* skeltal = GetMesh();
	if (skeltal)
	{
		skeltal->UnlinkAnimClassLayers(InAnimLayerClass);
		
		MY_LOG(LogTemp, Warning, TEXT("[%s] Successfully unlinked anim layer: %s"), 
			bIsServer ? TEXT("Server") : TEXT("Client"),
			*InAnimLayerClass->GetName());
	}
	else
	{
		MY_LOG(LogTemp,Error,TEXT("MeshComp is nullptr"));
	}
}

EMoveDirection4 AEntityBase::GetMoveDirection4(FVector WorldDirection) const
{
	if (WorldDirection.IsNearlyZero())
	{
		return EMoveDirection4::None;
	}

	// 캐릭터의 forward, right 벡터
	const FVector CharacterForward = GetActorForwardVector();
	const FVector CharacterRight = GetActorRightVector();

	// 입력 방향 정규화
	const FVector NormalizedDirection = WorldDirection.GetSafeNormal();

	// 내적을 이용해 각 방향 성분 계산
	const float ForwardDot = FVector::DotProduct(NormalizedDirection, CharacterForward);
	const float RightDot = FVector::DotProduct(NormalizedDirection, CharacterRight);

	// 더 큰 성분을 기준으로 방향 결정
	if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
	{
		return ForwardDot > 0 ? EMoveDirection4::Front : EMoveDirection4::Back;
	}
	else
	{
		return RightDot > 0 ? EMoveDirection4::Right : EMoveDirection4::Left;
	}
}

EMoveDirection8 AEntityBase::GetMoveDirection8(FVector WorldDirection) const
{
	if (WorldDirection.IsNearlyZero())
	{
		return EMoveDirection8::None;
	}

	// 캐릭터의 forward, right 벡터
	const FVector CharacterForward = GetActorForwardVector();
	const FVector CharacterRight = GetActorRightVector();

	// 입력 방향 정규화
	const FVector NormalizedDirection = WorldDirection.GetSafeNormal();

	// 캐릭터 기준 각도 계산 (-180 ~ 180도)
	const float Angle = FMath::Atan2(
		FVector::DotProduct(NormalizedDirection, CharacterRight),
		FVector::DotProduct(NormalizedDirection, CharacterForward)
	) * 180.0f / PI;

	// 8방향으로 분할 (각 방향당 45도)
	if (Angle >= -22.5f && Angle < 22.5f)
		return EMoveDirection8::Front;
	else if (Angle >= 22.5f && Angle < 67.5f)
		return EMoveDirection8::FrontRight;
	else if (Angle >= 67.5f && Angle < 112.5f)
		return EMoveDirection8::Right;
	else if (Angle >= 112.5f && Angle < 157.5f)
		return EMoveDirection8::BackRight;
	else if (Angle >= 157.5f || Angle < -157.5f)
		return EMoveDirection8::Back;
	else if (Angle >= -157.5f && Angle < -112.5f)
		return EMoveDirection8::BackLeft;
	else if (Angle >= -112.5f && Angle < -67.5f)
		return EMoveDirection8::Left;
	else // -67.5f ~ -22.5f
		return EMoveDirection8::FrontLeft;
}

void AEntityBase::BeginPlay()
{
	Super::BeginPlay();

	if(UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("State.Dead.Start"),
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnDeathStartTagChanged);
		ASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("State.Dead.End"),
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnDeathEndTagChanged);
	}
}

void AEntityBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEntityBase, FocusedObject);
}

void AEntityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEntityBase::Server_SetFocusedObject_Implementation(AActor* InFocusedObject)
{

	FocusedObject = InFocusedObject;
	// GameplayTag 추가
	/*
	* TODO: 왜인지 모르겠지만, AddReplicatedLooseGameplayTag는 작동을 잘 안함. -> Effect로 부여하자
	*/ 
	GetDDSAbilitySystemComponent()->Multicast_AddLooseGameplayTag(DDSGameplayTags::Shared_State_LockedOn);
	// GetDDSAbilitySystemComponent()->ApplyGameplayEffectToSelf
}

void AEntityBase::Server_ClearFocusedObject_Implementation()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	FocusedObject = nullptr;

	GetDDSAbilitySystemComponent()->Multicast_RemoveLooseGameplayTag(DDSGameplayTags::Shared_State_LockedOn);
	
	// 카메라를 정상 상태로 복구
	OnRep_FocusedObject();
}

void AEntityBase::OnRep_FocusedObject()
{
	// FocusedObject가 클리어되었을 때 카메라 복구
	if (!FocusedObject)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void AEntityBase::OnDeathStartTagChanged(const FGameplayTag ChangedTag, int32 NumberOfTag)
{
	MY_LOG(LogTemp, Warning, TEXT("Death"));

}

void AEntityBase::OnDeathEndTagChanged(const FGameplayTag ChangedTag, int32 NumberOfTag)
{
	MY_LOG(LogTemp, Warning, TEXT("Death End"));
	Destroy();
}
