#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Ghost_SpecOps/Components/PlayerCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

APlayerCharacter::APlayerCharacter()
{
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

	CombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::OnRunButtonPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::OnRunButtonReleased);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::OnCrouchButtonPressed);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::OnFireButtonReleased);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::OnAimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::OnAimButtonReleased);

	PlayerInputComponent->BindAction("AimDownSights", IE_Pressed, this, &APlayerCharacter::OnADSButtonPressed);
	PlayerInputComponent->BindAction("AimDownSights", IE_Released, this, &APlayerCharacter::OnADSButtonReleased);

	// PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &APlayerCharacter::OnProneButtonPressed);

}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CalculateAimOffset(DeltaSeconds);
}



void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(CombatComponent)
	{
		CombatComponent->PlayerCharacter = this;
	}
	
}

//------------------------------------------- Movement ------------------------------------------------------------

void APlayerCharacter::MoveForward(float InAxisValue)
{
	if(Controller && InAxisValue)
	{
		if(bIsRunning && InAxisValue < 0.f)
		{
			InAxisValue *= -1;
		}
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, InAxisValue);
	}
}

void APlayerCharacter::MoveRight(float InAxisValue)
{
	if(!bIsRunning)
	{
		if(Controller && InAxisValue && !bIsProne)
		{
			const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
			const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
			AddMovementInput(Direction, InAxisValue);
		}
		if(bIsProne)
		{
			AddControllerYawInput(InAxisValue);
		}	
	}
}

void APlayerCharacter::Turn(float InAxisValue)
{
	if(bIsRunning)
	{
		InAxisValue *= 0.2;
	}
	AddControllerYawInput(InAxisValue);
}

void APlayerCharacter::TurnInPlace(float DeltaTime)
{
	if(AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
		// AddControllerYawInput(10.f);
	}
	else if(AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
		// AddControllerYawInput(-10.f);
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

//-------------------------------------------- Fire ---------------------------------------------------------------

void APlayerCharacter::OnFireButtonPressed()
{
	if(CombatComponent && !bIsRunning)
	{
		CombatComponent->FireButtonPressed(true);
	}
}

void APlayerCharacter::OnFireButtonReleased()
{
	if (CombatComponent)
	{
		CombatComponent->FireButtonPressed(false);
	}
}

void APlayerCharacter::CalculateAimOffset(float DeltaTime)
{
	if(!CombatComponent || !CurrentWeapon)
	{
		return;
	}
	
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	// bool bIsInAir = GetCharacterMovement()->IsFalling();
	
	if(Speed == 0.f)
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if(TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
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

	AO_Pitch = FMath::Clamp(AO_Pitch, -70.f, 70.f);
	
	UE_LOG(LogTemp, Warning, TEXT("Pitch: %f"), AO_Pitch);
	
	if(AO_Pitch >= 90.f && !IsLocallyControlled())
	{
		//Map pitch from [270, 360) to [-90, 0)
		FVector2d InRage(270.f, 360.f);
		FVector2d OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRage, OutRange, AO_Pitch);
	}
}

//---------------------------------------- Crouch & Prone ---------------------------------------------------------

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

void APlayerCharacter::OnProneButtonPressed()
{
	if (bIsProne)
	{
		FRotator ActorRotator = GetActorRotation();
		FVector UpVector = (UKismetMathLibrary::GetUpVector(ActorRotator) * FVector(0.f, 0.f, 180.f)) + GetActorLocation();

		FHitResult HitResult;
		bool bCanStandUp;
		bCanStandUp = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), UpVector, ECC_Visibility);

		bIsProne = false;
		bIsStanding = true;

		GetCharacterMovement()->MaxWalkSpeed = 0.0f;

		//"delay"
		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([&]
		{
			GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
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

//---------------------------------------------- Aim --------------------------------------------------------------

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
	if(!bIsRunning)
	{
		bIsAiming = bInIsAiming;
		Server_SetAiming(bInIsAiming);
		GetCharacterMovement()->MaxWalkSpeed = (bIsAiming && !bIsADS) ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void APlayerCharacter::Server_SetAiming_Implementation(bool bInIsAiming)
{
	bIsAiming = bInIsAiming;
	GetCharacterMovement()->MaxWalkSpeed = (bIsAiming && !bIsADS) ? AimWalkSpeed : BaseWalkSpeed;
}

bool APlayerCharacter::Server_SetAiming_Validate(bool bInIsAiming)
{
	return true;
}

//----------------------------------------------- ADS -----------------------------------------------------------------

void APlayerCharacter::OnADSButtonPressed()
{
	SetAds(true);
	SetAiming(true);
}

void APlayerCharacter::OnADSButtonReleased()
{
	SetAds(false);
	SetAiming(false);
}

void APlayerCharacter::SetAds(bool bInAds)
{
	bIsADS = bInAds;

	if(bIsADS && CurrentWeapon && CurrentWeapon->GetWeaponMesh())
	{
		CameraComponent->AttachToComponent(CurrentWeapon->GetWeaponMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ADS_Socket"));
		SpringArmComponent->bUsePawnControlRotation = false;
		CameraComponent->bUsePawnControlRotation = true;
	}
	else
	{
		CameraComponent->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, USpringArmComponent::SocketName);
		SpringArmComponent->bUsePawnControlRotation = true;
		CameraComponent->bUsePawnControlRotation = false;
	}
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
			GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		}
		Server_SetRunning(bInIsRunning);
	}
}

void APlayerCharacter::Server_SetRunning_Implementation(bool bInIsRunning)
{
	bIsRunning = bInIsRunning;
}

bool APlayerCharacter::Server_SetRunning_Validate(bool bInIsrunning)
{
	return true;
}

