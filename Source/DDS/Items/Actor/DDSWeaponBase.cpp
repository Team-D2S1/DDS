// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DDSWeaponBase.h"

#include "Character/Player/PlayerBase.h"
#include "Components/BoxComponent.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "ETC/CustomLog.h"
#include "ETC/DDSFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADDSWeaponBase::ADDSWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

}
void ADDSWeaponBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* OwingPawn = GetInstigator();
	if (OwingPawn == nullptr)
	{
		MY_CLOG_DISPLAY_NET( FColor::Red, HasAuthority(), TEXT("Weapon %s OwingPawn is nullptr"), *GetName());
		return;
	}
	APawn* OtherPawn = Cast<APawn>(OtherActor);
	if (OtherPawn == nullptr)
	{
		MY_CLOG_DISPLAY_NET( FColor::Red, HasAuthority(), TEXT("Weapon %s OtherPawn is nullptr"), *GetName());
		return;
	}
	if (OtherPawn != OwingPawn)
	{
		if (UDDSFunctionLibrary::IsTargetHostile(OwingPawn,OtherPawn))
		{
			MY_CLOG_DISPLAY_NET( FColor::Silver, HasAuthority(), TEXT("Weapon %s Begin Overlap with %s"), *GetName(), *OtherActor->GetName());
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
			if (!OnWeaponHitTarget.IsBound())
			{
				MY_ERROR_DISPLAY(TEXT("OnWeaponHitTarget is not bound"));
			}
		}
	}
}

void ADDSWeaponBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* OwingPawn = GetInstigator();
	if (OwingPawn == nullptr)
		return;
	APawn* OtherPawn = Cast<APawn>(OtherActor);
	if (OtherPawn == nullptr)
		return;
	if (OtherPawn != OwingPawn)
	{
		// DEBUG_CLOG_DISPLAY_NET( FColor::Silver, HasAuthority(), TEXT("Weapon %s End Overlap with %s"), *GetName(), *OtherActor->GetName());
		OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
	}
}
void ADDSWeaponBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADDSWeaponBase, ParentItemId);
	DOREPLIFETIME(ADDSWeaponBase, OwnerPawn);
	DOREPLIFETIME(ADDSWeaponBase, WeaponTag);
}



void ADDSWeaponBase::SetOwnerPawn(APawn* InOwnerPawn)
{
	OwnerPawn = InOwnerPawn;
}

void ADDSWeaponBase::OnRep_OwnerPawn()
{
	if (OwnerPawn)
	{
		APlayerBase* Player = Cast<APlayerBase>(OwnerPawn);
		if (Player)
		{
			Player->GetCombatComponent()->RegisterSpawnedWeapon(WeaponTag ,this);
		}
	}
}

void ADDSWeaponBase::SetWeaponTag(FGameplayTag InWeaponTag)
{
	WeaponTag = InWeaponTag;
}

void ADDSWeaponBase::OnRep_WeaponTag()
{
}
ADDSWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeapon(FGameplayTag InWeaponTag) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTag))
	{
		if (ADDSWeaponBase* const* res = CharacterCarriedWeaponMap.Find(InWeaponTag))
		{
			return *res;
		}
	}
	MY_LOG(LogTemp, Error, TEXT("Weapon Tag %s is not registered."), *InWeaponTag.ToString());
	return nullptr;
}

ADDSWeaponBase* UPawnCombatComponent::GetCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		MY_LOG(LogTemp, Error, TEXT("CurrentEquippedWeaponTag is not valid."));
		return nullptr;
	}
	return GetCharacterCarriedWeapon(CurrentEquippedWeaponTag);
}

