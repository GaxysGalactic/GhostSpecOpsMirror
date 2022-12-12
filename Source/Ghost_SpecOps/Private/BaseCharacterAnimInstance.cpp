// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterAnimInstance.h"

#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	bIsFalling = BaseCharacter->GetCharacterMovement()->IsFalling();

	TurningInPlace = BaseCharacter->GetTurningInPlace();

}
