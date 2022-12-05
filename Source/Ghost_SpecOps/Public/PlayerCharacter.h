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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RunSpeed;
	
	UPROPERTY(BlueprintReadWrite, Replicated, VisibleAnywhere)
	bool bIsProne;

	UPROPERTY(BlueprintReadWrite, Replicated, VisibleAnywhere)
	bool bIsStanding;
	
	//-----------------------------------------------------------------------------------------------------------------
	
	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	//-----------------------------------------------------------------------------------------------------------------

	void CrouchButtonPressed();

	void ProneButtonPressed();

	//-----------------------------------------------------------------------------------------------------------------

	void AimButtonPressed();
	void AimButtonReleased();
	void SetAiming(bool bInIsAiming);

	//-----------------------------------------------------------------------------------------------------------------

	void RunButtonPressed();
	void RunButtonReleased();
	void SetRunning(bool bInIsRunning);

	//-----------------------------------------------------------------------------------------------------------------

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAiming(bool bInIsAiming);//rpc

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetRunning(bool bInIsRunning);

	//-----------------------------------------------------------------------------------------------------------------

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//-----------------------------------------------------------------------------------------------------------------

	virtual FVector GetPawnViewLocation() const override;
	
};
