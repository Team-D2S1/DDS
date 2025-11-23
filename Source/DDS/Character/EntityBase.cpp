// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "EntityBase.h"

#include "DDSGameplayTags.h"
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
}

void AEntityBase::OnRep_FocusedObject()
{
	
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
