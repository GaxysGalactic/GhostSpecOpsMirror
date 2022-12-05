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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFrightened;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEssential;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead;
	
};
