#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;
class UAnimationAsset;
class UTexture2D;

UCLASS()
class GHOST_SPECOPS_API AWeapon : public AActor
{
private:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Weapon properties")
	UAnimationAsset* FireAnimation;

	// Zoom variables

	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;
	
	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;

protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon properties")
	USkeletalMeshComponent* WeaponSkeletalMesh; // if animated

	UPROPERTY(EditDefaultsOnly)
	float ShootDistance;

	UPROPERTY(EditDefaultsOnly)
	float ShootDamage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> DamageType;
	
	virtual void BeginPlay() override;

public:	
	AWeapon();
	virtual void Fire(const FVector& InTarget);
	
	// Weapon crosshair testures
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsCenter;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsTop;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsRight;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsBottom;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsLeft;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	FORCEINLINE float GetZoomedFOV() const { return  ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return  ZoomedFOV; }

};
