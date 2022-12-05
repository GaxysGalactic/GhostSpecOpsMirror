// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CivilianCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GHOST_SPECOPS_API ACivilianCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:

	ACivilianCharacter();

protected:

	/** Used for the "Cower" state / animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFrightened;

	/** Indicates if being alive is essential to mission success. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEssential;

	/** Used for death state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead;
	
};
