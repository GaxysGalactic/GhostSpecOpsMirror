#pragma once

#include "CoreMinimal.h"
#include "Ghost_SpecOps/BaseCharacter/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"


//Forward declaration
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;

UCLASS()
class GHOST_SPECOPS_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return CameraComponent; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent_ADS;

	UPROPERTY(EditAnywhere, Category = Camera)
	AActor* ViewActor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	APlayerController* PlayerController;

protected:

	FRotator StartingAimRotation;

	//------------------------------------------- Movement ------------------------------------------------------------
	
	void MoveForward(float InAxisValue);

	void MoveRight(float InAxisValue);

	void Turn(float InAxisValue);
	
	void TurnInPlace(float InAxisValue);
	
	//-------------------------------------------- Fire ---------------------------------------------------------------

	void OnFireButtonPressed();
	
	void OnFireButtonReleased();

	void CalculateAimOffset(float DeltaTime);

	//---------------------------------------- Crouch & Prone ---------------------------------------------------------

	void OnCrouchButtonPressed();

	void OnProneButtonPressed();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnProneButtonPressed();

	//---------------------------------------------- Aim --------------------------------------------------------------

	void OnAimButtonPressed();
	void OnAimButtonReleased();
	void SetAiming(bool bInIsAiming);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAiming(bool bInIsAiming);

	//---------------------------------------------- ADS --------------------------------------------------------------

	void OnADSButtonPressed();
	void OnADSButtonReleased();
	void SetAds(bool bInAds);

	//---------------------------------------------- Run --------------------------------------------------------------

	void OnRunButtonPressed();
	void OnRunButtonReleased();
	void SetRunning(bool bInIsRunning);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetRunning(bool bInIsRunning);

};
