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
	if(FireAnimation && ImpactParticles)
	{
		WeaponSkeletalMesh->PlayAnimation(FireAnimation, false);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, InTarget);// not working
	}
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}



