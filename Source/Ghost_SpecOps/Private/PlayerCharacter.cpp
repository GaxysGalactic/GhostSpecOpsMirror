#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	bIsAiming = false;
	bIsRunning = false;
	bIsCrouched = false;

	WalkSpeed = 600.f;
	RunSpeed = 900.f;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

//-------------------------------------------- Movement ---------------------------------------------------------------

void APlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

//-------------------------------------------- Crouch ---------------------------------------------------------------

void APlayerCharacter::CrouchButtonPressed()
{
	if(bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

//-------------------------------------------- Aim ---------------------------------------------------------------

void APlayerCharacter::AimButtonPressed()
{
	SetAiming(true);
}

void APlayerCharacter::AimButtonReleased()
{
	SetAiming(false);
}

void APlayerCharacter::SetAiming(bool bInIsAiming)
{
	bIsAiming = bInIsAiming;
	Server_SetAiming(bInIsAiming);// ojo no llamar al _Implemantation
}



//-------------------------------------------- Run ---------------------------------------------------------------

void APlayerCharacter::RunButtonPressed()
{
	SetRunning(true);
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

}

void APlayerCharacter::RunButtonReleased()
{
	SetRunning(false);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::SetRunning(bool bInIsRunning)
{
	bIsRunning = bInIsRunning;
	Server_SetRunning(bInIsRunning);
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::CrouchButtonPressed);

	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StartFire);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::AimButtonReleased);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::RunButtonPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::RunButtonReleased);



}

FVector APlayerCharacter::GetPawnViewLocation() const
{
	if(IsValid(CameraComponent))
	{
		return CameraComponent->GetComponentLocation();
	}
	
	return Super::GetPawnViewLocation();
}

//----------------------------------Server Implementations ------------------------------------------------------------

void APlayerCharacter::Server_SetAiming_Implementation(bool bInIsAiming)
{
	bIsAiming = bInIsAiming;
}

void APlayerCharacter::Server_SetRunning_Implementation(bool bInIsRunning)
{
	bIsRunning = bInIsRunning;
}

//------------------------------------ Server validates ---------------------------------------------------------------
//checks for hacks xd <- if return false, 

bool APlayerCharacter::Server_SetAiming_Validate(bool bInIsAiming)
{
	return true;
}

bool APlayerCharacter::Server_SetRunning_Validate(bool bInIsrunning)
{
	return true;
}