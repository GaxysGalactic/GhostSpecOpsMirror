#include "Weapon.h"
#include "Ghost_SpecOps/Ghost_SpecOps.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;

	ShootDistance = 10000.f;
	ShootDamage = 20.f;
}

void AWeapon::Fire(const FVector& InTarget)
{
	if(FireAnimation)
	{
		WeaponSkeletalMesh->PlayAnimation(FireAnimation, false);
	}
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}



