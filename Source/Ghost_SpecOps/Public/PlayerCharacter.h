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

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void StartCrouch();

	void EndCrouch();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
	
};
