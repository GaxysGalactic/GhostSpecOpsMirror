#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class UParticleSystemComponent;

UCLASS()
class GHOST_SPECOPS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// UFUNCTION()
	// virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* HitActor, UPrimitiveComponent OtherComp, FVector NormalImpolse, const FHitResult& InHit);

private:

	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystem* Tracer;

	UParticleSystemComponent* TracerComponent;
};
