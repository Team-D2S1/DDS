// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "EntityBase.h"

#include "DDSGameplayTags.h"
#include "ETC/CustomLog.h"
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameAbilitySystem/DDSAttributeSet.h"
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

void AEntityBase::BeginPlay()
{
	Super::BeginPlay();
	
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
