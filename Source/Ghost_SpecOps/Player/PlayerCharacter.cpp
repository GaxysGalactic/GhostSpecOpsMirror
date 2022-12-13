#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

APlayerCharacter::APlayerCharacter()
{
	RunSpeed = 900.f;
	WalkSpeed = 600.f;
	AimWalkSpeed = 400.f;
	ProneSpeed = 100.f;

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

void APlayerCharacter::PlayFireMontage(bool bInAiming)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = bInAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::OnRunButtonPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::OnRunButtonReleased);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::OnCrouchButtonPressed);

	// PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::OnFireButtonReleased);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::OnAimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::OnAimButtonReleased);

	// PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &APlayerCharacter::OnProneButtonPressed);

}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CalculateAimOffset(DeltaSeconds);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APlayerCharacter, bIsProne)
	DOREPLIFETIME(APlayerCharacter, bIsStanding);
}

//-------------------------------------------- Movement ---------------------------------------------------------------

void APlayerCharacter::OnFireButtonPressed()
{
	PlayFireMontage(bIsAiming);
}

void APlayerCharacter::OnFireButtonReleased()
{
	
}

void APlayerCharacter::CalculateAimOffset(float DeltaTime)
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	
	if(Speed == 0.f)
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if(TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = false;
		TurnInPlace(DeltaTime);
	}
	if(Speed > 0.f)
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;
	if(AO_Pitch >= 90.f && !IsLocallyControlled())
	{
		//Map pitch from [270, 360) to [-90, 0)
		FVector2d InRage(270.f, 360.f);
		FVector2d OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRage, OutRange, AO_Pitch);
	}
}

void APlayerCharacter::TurnInPlace(float DeltaTime)
{
	if(AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if(AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}

	if(TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;
		if(FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	if(Controller && AxisValue)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, AxisValue);
	}
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

//---------------------------------------------- Crouch ---------------------------------------------------------------

void APlayerCharacter::OnCrouchButtonPressed()
{
	if(!bIsProne)
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
}

//---------------------------------------------- Prone ---------------------------------------------------------------

void APlayerCharacter::OnProneButtonPressed()
{
	if (bIsProne)
	{
		bIsProne = false;
		bIsStanding = true;

		GetCharacterMovement()->MaxWalkSpeed = 0.0f;

		//"delay"
		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([&]
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		});

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, 1.3f, false);

	}
	else
	{
		bIsProne = true;
		bIsStanding = false;

		GetCharacterMovement()->MaxWalkSpeed = 0.0f;

		//"delay"
		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([&]
		{
			GetCharacterMovement()->MaxWalkSpeed = ProneSpeed;
		});

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, TimerCallback, 1.3f, false);

	}
	
	if(!HasAuthority())
	{
		Server_OnProneButtonPressed();
	}
}

void APlayerCharacter::Server_OnProneButtonPressed_Implementation()
{
	OnProneButtonPressed();
}

bool APlayerCharacter::Server_OnProneButtonPressed_Validate()
{
	return true;
}

//----------------------------------------------- Aim -----------------------------------------------------------------

void APlayerCharacter::OnAimButtonPressed()
{
	SetAiming(true);
}

void APlayerCharacter::OnAimButtonReleased()
{
	SetAiming(false);
}

void APlayerCharacter::SetAiming(bool bInIsAiming)
{
	bIsAiming = bInIsAiming;
	Server_SetAiming(bInIsAiming);
	GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : WalkSpeed;
}

//----------------------------------------------- Run -----------------------------------------------------------------

void APlayerCharacter::OnRunButtonPressed()
{
	SetRunning(true);
}

void APlayerCharacter::OnRunButtonReleased()
{
	SetRunning(false);
}

void APlayerCharacter::SetRunning(bool bInIsRunning)
{
	if (!bIsProne || bIsCrouched)
	{
		bIsRunning = bInIsRunning;
		if (bIsRunning)
		{
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
		Server_SetRunning(bInIsRunning);
	}
}

//---------------------------------------------------------------------------------------------------------------------

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
	GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : WalkSpeed;
}

void APlayerCharacter::Server_SetRunning_Implementation(bool bInIsRunning)
{
	bIsRunning = bInIsRunning;
}



//------------------------------------ Server validates ---------------------------------------------------------------

bool APlayerCharacter::Server_SetAiming_Validate(bool bInIsAiming)
{
	return true;
}

bool APlayerCharacter::Server_SetRunning_Validate(bool bInIsrunning)
{
	return true;
}

