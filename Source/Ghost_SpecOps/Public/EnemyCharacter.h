// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GHOST_SPECOPS_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AEnemyCharacter();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanSeePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
	
};
