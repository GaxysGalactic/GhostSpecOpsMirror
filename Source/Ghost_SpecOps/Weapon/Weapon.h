#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;
class UAnimationAsset;

UCLASS()
class GHOST_SPECOPS_API AWeapon : public AActor
{
private:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Weapon properties")
	UAnimationAsset* FireAnimation;
	
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

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

};
