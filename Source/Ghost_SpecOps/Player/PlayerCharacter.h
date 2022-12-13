#pragma once

#include "CoreMinimal.h"
#include "Ghost_SpecOps/BaseCharacter/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

#define TRACE_LENGTH 80000.f

//Forward declaration
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;

UCLASS()
class GHOST_SPECOPS_API APlayerCharacter : public ABaseCharacter
{
private:

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* FireWeaponMontage;

	FVector HitTarget;
	
	GENERATED_BODY()

	APlayerCharacter();

public:

	void PlayFireMontage(bool bInAiming);

protected:

	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RunSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ProneSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AimWalkSpeed;

	FRotator StartingAimRotation;

	//------------------------------------- Functions -----------------------------------------------------------------

	void OnFireButtonPressed();
	
	void OnFireButtonReleased();

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
		
	void CalculateAimOffset(float DeltaTime);

	void TurnInPlace(float DeltaTime);
	
	//-----------------------------------------------------------------------------------------------------------------
	
	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	//-----------------------------------------------------------------------------------------------------------------

	void OnCrouchButtonPressed();

	void OnProneButtonPressed();

	//-----------------------------------------------------------------------------------------------------------------

	void OnAimButtonPressed();
	void OnAimButtonReleased();
	void SetAiming(bool bInIsAiming);

	//-----------------------------------------------------------------------------------------------------------------

	void OnRunButtonPressed();
	void OnRunButtonReleased();
	void SetRunning(bool bInIsRunning);

	//---------------------------------------------- RPC --------------------------------------------------------------

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAiming(bool bInIsAiming);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetRunning(bool bInIsRunning);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnProneButtonPressed();

	//-----------------------------------------------------------------------------------------------------------------

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//-----------------------------------------------------------------------------------------------------------------

	virtual FVector GetPawnViewLocation() const override;

	virtual void Tick(float DeltaSeconds) override;
	
};
