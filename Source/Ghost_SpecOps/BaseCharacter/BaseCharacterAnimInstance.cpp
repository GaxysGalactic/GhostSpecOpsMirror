#include "BaseCharacterAnimInstance.h"

#include "BaseCharacter.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBaseCharacterAinmInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UBaseCharacterAinmInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(!BaseCharacter)
	{
		BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	}
	if(!BaseCharacter)
	{
		return;
	}

	FVector Velocity = BaseCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	
	const FRotator ActorRotation = BaseCharacter->GetActorRotation();
	CharacterDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, ActorRotation);
	

	// bIsInAir = BaseCharacter->GetCharacterMovement()->IsFalling();
	// bIsAccelerating = BaseCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	EquippedWeapon = BaseCharacter->GetWeapon();
	bIsCrouched = BaseCharacter->bIsCrouched;
	bIsAiming = BaseCharacter->bIsAiming;
	bIsProne = BaseCharacter->bIsProne;
	bIsStanding = BaseCharacter->bIsStanding;
	bIsRunning = BaseCharacter->bIsRunning;
	TurningInPlace = BaseCharacter->GetTurningInPlace();


	//offset Yaw for strafing
	// FRotator AimRotation = BaseCharacter->GetBaseAimRotation();
	// FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BaseCharacter->GetVelocity());
	// FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	// DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	// YawOffset = DeltaRotation.Yaw;

	
	// CharacterRotationLastFrame = CharacterRotation;
	// CharacterRotation = BaseCharacter->GetActorRotation();
	// const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	// const float Target = Delta.Yaw / DeltaTime;
	// const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	// Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = BaseCharacter->GetAO_Yaw();
	AO_Pitch = BaseCharacter->GetAO_Pitch();
	 

	if (EquippedWeapon && EquippedWeapon->GetWeaponMesh() && BaseCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		BaseCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		if(BaseCharacter->IsLocallyControlled())
		{
			bIsLocallyControlled = true;
			FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("hand_r"), RTS_World);
			RightHandRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BaseCharacter->GetHitTarget()));

			//...for debug
			// FTransform MuzzleTipTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), RTS_World);
			// FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
			// DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), MuzzleTipTransform.GetLocation() + MuzzleX * 1000.f, FColor::Red);
			// DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), BaseCharacter->GetHitTarget(), FColor::Orange);
			
		}
		
	}

}
