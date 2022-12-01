#include "BaseCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	
	bReplicates = true;
	SetReplicatingMovement(true);

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
	DOREPLIFETIME(ABaseCharacter, CharacterPitch)

}



