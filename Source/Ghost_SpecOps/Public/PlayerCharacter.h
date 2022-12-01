#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

//Forward declaration
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class GHOST_SPECOPS_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	APlayerCharacter();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly)
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly)
	float RunSpeed;

	//-----------------------------------------------------------------------------------------------------------------
	
	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	//-----------------------------------------------------------------------------------------------------------------

	void CrouchButtonPressed();

	//-----------------------------------------------------------------------------------------------------------------

	void AimButtonPressed();
	void AimButtonReleased();
	void SetAiming(bool bInIsAiming);

	//-----------------------------------------------------------------------------------------------------------------

	void RunButtonPressed();
	void  RunButtonReleased();
	void SetRunning(bool bInIsRunning);

	//-----------------------------------------------------------------------------------------------------------------

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAiming(bool bInIsAiming);//rpc

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetRunning(bool bInIsRunning);

	//-----------------------------------------------------------------------------------------------------------------

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//-----------------------------------------------------------------------------------------------------------------

	virtual FVector GetPawnViewLocation() const override;
	
};
