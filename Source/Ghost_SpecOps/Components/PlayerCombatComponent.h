#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ghost_SpecOps/Player/GhostPlayerController.h"
#include "Ghost_SpecOps/Player/PlayerCharacter.h"
#include "PlayerCombatComponent.generated.h"

#define TRACE_LENGTH 80000.f

class AWeapon;
class ABaseCharacter;
class AGhostPlayerController;
class APlayerHUD;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GHOST_SPECOPS_API UPlayerCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerCombatComponent();

	friend class APlayerCharacter;
	friend class ABaseCharacter;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void FireButtonPressed(bool bInState);

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& InTraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& InTraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

private:
	
	APlayerCharacter* PlayerCharacter;
	AGhostPlayerController* Controller;
	APlayerHUD* HUD;

	bool bFireButtonPressed;

	// HUD && crosshairs
	float CrosshairVelocityFactor;

	FVector HitTarget;

private:

	/*
	 * Aiming && FOV
	 */

	float DefaulfFOV;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);




};
