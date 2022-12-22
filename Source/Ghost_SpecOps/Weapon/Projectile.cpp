#include "Projectile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMuvementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
		);
	}

	// if(HasAuthority())
	// {
	// 	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	// }
}

// void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent OtherComp, FVector NormalImpolse, const FHitResult& InHit)
// {
// 	
// }

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

