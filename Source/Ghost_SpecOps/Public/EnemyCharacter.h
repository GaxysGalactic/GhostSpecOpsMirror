// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "../../../../../../../../../Program Files/Epic Games/UE_5.1/Engine/Plugins/Animation/RigLogic/Source/RigLogicLib/Public/trimd/Macros.h"
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

	/** Path for the enemy to follow (or guard location) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APatrolPath* PatrolPath;

	/** Current index in patrol path */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PatrolIndex;

	/** Enemy sees player or remembers seeing them */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanSeePlayer;

	/** Current health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

public:

	/** Returns the patrol path */
	FORCE_INLINE APatrolPath* GetPatrolPath() const { return PatrolPath; }

	/** Returns the patrol index */
	FORCE_INLINE int32 GetPatrolIndex() const { return PatrolIndex; }

	void UpdatePatrolIndex();
	
};
