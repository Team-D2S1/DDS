// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DDSWeaponBase.h"
#include "GameFramework/Actor.h"
#include "WeaponBladePart.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnWeaponBladePartBeginOverlapDelegate, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, const FHitResult&, SweepResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnWeaponBladePartEndOverlapDelegate, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex);

UCLASS()
class DDS_API AWeaponBladePart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBladePart();


	UFUNCTION(BlueprintCallable, Category = "WeaponBladePart")
	UBoxComponent* GetBladePartCollisionBox() const { return BladePartCollisionBox; }

    UPROPERTY(BlueprintAssignable, Category = "WeaponBladePart")
	FOnWeaponBladePartBeginOverlapDelegate OnWeaponBladePartBeginOverlap;
	UPROPERTY(BlueprintAssignable, Category = "WeaponBladePart")
	FOnWeaponBladePartEndOverlapDelegate OnWeaponBladePartEndOverlap;
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponBladePart")
	TObjectPtr<UStaticMeshComponent> BladePartMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponBladePart")
	TObjectPtr<UBoxComponent> BladePartCollisionBox;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:	
	
};