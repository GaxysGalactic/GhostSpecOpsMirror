// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Ghost_SpecOps/Types/TurningInPlace.h"
#include "BaseCharacterAnimInstance.generated.h"

class ABaseCharacter;

UCLASS()
class GHOST_SPECOPS_API UBaseCharacterAinmInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = true))
	ABaseCharacter* BaseCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = true))
	float Speed;

	bool bIsFalling;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = true))
	ETurningInPlace TurningInPlace;

};
