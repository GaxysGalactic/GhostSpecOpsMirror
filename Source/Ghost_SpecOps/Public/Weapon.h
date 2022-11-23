#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UStaticMeshComponent;

UCLASS()
class GHOST_SPECOPS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

protected:

	UStaticMeshComponent* WeaponMesh;
	
	virtual void BeginPlay() override;

public:	

};
