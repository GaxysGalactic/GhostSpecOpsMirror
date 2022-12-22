#include "BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Ghost_SpecOps/Components/PlayerCombatComponent.h"
#include "Net/UnrealNetwork.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	RunSpeed = 900.f;
	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 400.f;
	ProneSpeed = 100.f;

	bIsAlive = true;

	

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	
	bReplicates = true;
	SetReplicatingMovement(true);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, bIsAiming)
	DOREPLIFETIME(ABaseCharacter, bIsRunning)
	DOREPLIFETIME(ABaseCharacter, bIsProne)
	DOREPLIFETIME(ABaseCharacter, bIsStanding)

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

FVector ABaseCharacter::GetHitTarget() const
{
	if(CombatComponent)
	{
		return CombatComponent->HitTarget;
	}
	return FVector();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// if(Combat)
	// {
	// 	Combat->PlayerCharacter = this;
	// }
}

void ABaseCharacter::PlayFireMontage(bool bAiming) const
{
	if (!CombatComponent || !CurrentWeapon) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		const FName SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
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





