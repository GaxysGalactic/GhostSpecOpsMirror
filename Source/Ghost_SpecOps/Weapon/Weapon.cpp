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
// old
// void AWeapon::Fire()
// {
// 	AActor* MyOwner = GetOwner();
// 	if(MyOwner)
// 	{
// 		FVector EyeLocation;
// 		FRotator EyeRotation;
// 		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
//
// 		FVector ShootDirection = EyeRotation.Vector();
// 		
// 		FVector TraceEnd = EyeLocation + (ShootDirection * ShootDistance);
//
// 		FCollisionQueryParams QueryParams;
// 		QueryParams.AddIgnoredActor(MyOwner);
// 		QueryParams.AddIgnoredActor(this);
// 		QueryParams.bTraceComplex = true;
//
// 		FHitResult Hit;
//
// 		if(GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, WEAPON_COLLISION, QueryParams))
// 		{
// 			AActor* HitActor = Hit.GetActor();
// 			UGameplayStatics::ApplyPointDamage(
// 				HitActor,
// 				ShootDamage,
// 				ShootDirection,
// 				Hit,
// 				MyOwner->GetInstigatorController(),
// 				MyOwner,
// 				DamageType
// 			);
//
// 			DrawDebugLine(GetWorld(), EyeLocation, Hit.ImpactPoint, FColor::Red, false, 1.f, 0, 1.f);
// 		}
// 	}
// }

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



