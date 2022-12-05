#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

APlayerCharacter::APlayerCharacter()
{
	bIsAiming = false;
	bIsRunning = false;
	bIsCrouched = false;

	WalkSpeed = 600.f;
	RunSpeed = 900.f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->TargetArmLength = 600.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::RunButtonPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::RunButtonReleased);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::CrouchButtonPressed);

	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StartFire);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::AimButtonReleased);

	// PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &APlayerCharacter::ProneButtonPressed);

}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APlayerCharacter, bIsProne)
	DOREPLIFETIME(APlayerCharacter, bIsStanding);
}

//-------------------------------------------- Movement ---------------------------------------------------------------

void APlayerCharacter::MoveForward(float AxisValue)
{
	if(Controller && AxisValue)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, AxisValue);
	}
	// AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if(Controller && AxisValue)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, AxisValue);
	}
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

void APlayerCharacter::ProneButtonPressed()
{
	// if(bIsProne)
	// {
	// 	bIsProne = false;
	// 	bIsStanding = true;
	// }
	// else
	// {
	// 	bIsProne = true;
	// 	bIsStanding = true;
	// }
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