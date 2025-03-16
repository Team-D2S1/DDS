// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "SpawnWeaponAbility.h"

#include "Components/Combat/PawnCombatComponent.h"
#include "ETC/CustomLog.h"
#include "Items/Weapons/DDSWeaponBase.h"
#include "UniversalObjectLocators/UniversalObjectLocatorUtils.h"

void USpawnWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthority(&ActivationInfo))
	{
		return;
	}
	MY_LOG(LogTemp, Log, TEXT("[%s] SpawnWeaponAbility Activated (handle : %s)"), *ActorInfo->AvatarActor->GetName(), *Handle.ToString());
	AActor* Owner = ActorInfo->AvatarActor.Get();
	// 1. 무기 소환
	// 2. 소켓에 장착
	ADDSWeaponBase* Weapon = Owner->GetWorld()->SpawnActor<ADDSWeaponBase>(WeaponToSpawn, FTransform::Identity);
	if (!Weapon)
	{
		MY_LOG(LogTemp, Error, TEXT("Weapon is nullptr"));
		return;
	}
	
	USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	Weapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

	GetPawnCombatComponentFromActorInfo()->RegisterSpawnedWeapon(WeaponTag, Weapon, bRegisterAsEquippedWeapon);

	//클라레플리케이션
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
