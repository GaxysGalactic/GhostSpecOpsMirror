#include "BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Ghost_SpecOps/Components/PlayerCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
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

	DOREPLIFETIME(ABaseCharacter, bIsAiming);
	DOREPLIFETIME(ABaseCharacter, bIsRunning);
	DOREPLIFETIME(ABaseCharacter, bIsProne);
	DOREPLIFETIME(ABaseCharacter, bIsStanding);
	DOREPLIFETIME(ABaseCharacter, BaseHealth);

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

void ABaseCharacter::OnRep_Health()
{
	//function called when health is replicated
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
}

void ABaseCharacter::PlayFireMontage(bool bAiming) const
{
	if (!CombatComponent || !CurrentWeapon || !CurrentWeapon->GetWeaponMesh()) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	FTransform SocketTransform{};

	const UStaticMeshSocket* BarrelSocket = CurrentWeapon->GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	if (AnimInstance && FireWeaponMontage && CurrentWeapon->GetFireSound() && CurrentWeapon->GetMuzzleFlash() && BarrelSocket)
	{
		BarrelSocket->GetSocketTransform(SocketTransform, CurrentWeapon->GetWeaponMesh());
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			CurrentWeapon->GetMuzzleFlash(),
			SocketTransform
			);
		
		UGameplayStatics::PlaySound2D(this, CurrentWeapon->GetFireSound());
		
		AnimInstance->Montage_Play(FireWeaponMontage);
		const FName SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}





