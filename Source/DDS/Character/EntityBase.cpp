// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "EntityBase.h"

#include "ETC/CustomLog.h"
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "GameAbilitySystem/DDSAttributeSet.h"

AEntityBase::AEntityBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEntityBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (ensure(EntityStartUpDataBase.IsNull()))
	{
		MY_LOG(LogTemp,Error,TEXT("Forgot to assign start up data to %s"),*GetName())
	}
}

UAbilitySystemComponent* AEntityBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AEntityBase::GetAttributeSet() const
{
	return AttributeSet;
}

void AEntityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEntityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

