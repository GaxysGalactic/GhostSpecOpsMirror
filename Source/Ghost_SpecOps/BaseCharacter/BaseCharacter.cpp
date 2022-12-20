#include "BaseCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"

ABaseCharacter::ABaseCharacter()
{
	bIsAiming = false;
	bIsRunning = false;
	bIsCrouched = false;
	
	PrimaryActorTick.bCanEverTick = true;
	ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	
	bReplicates = true;
	SetReplicatingMovement(true);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

}

void ABaseCharacter::StartFire()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(StartingWeaponClass))
	{
		CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(StartingWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if(IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
		}
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, bIsAiming)
	DOREPLIFETIME(ABaseCharacter, bIsRunning)
}

void ABaseCharacter::MulticastPlayAnimMontage_Implementation(UAnimMontage* Montage)
{
	PlayAnimMontage(Montage);
	//GetMesh()->GetAnimInstance()->OnMontageEnded.Broadcast();
}

bool ABaseCharacter::MulticastPlayAnimMontage_Validate(UAnimMontage* Montage)
{
	return true;
}



