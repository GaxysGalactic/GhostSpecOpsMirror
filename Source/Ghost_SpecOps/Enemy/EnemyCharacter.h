// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter/BaseCharacter.h"
#include "Perception/AIPerceptionTypes.h"
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

private:

	virtual void BeginPlay() override;

	/** Starts the State Tree Logic */
	void StartStateTree() const;

	/** Taking damage */
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	/** Process visual stimuli */
	void ProcessVision(AActor* Actor, FAIStimulus Stimulus);

	/** Process incoming Perception Stimuli */
	UFUNCTION()
	void ProcessStimuli(AActor* Actor, FAIStimulus Stimulus);

	/** Send Alert event to State Tree */
	void Alert() const;

	/** Send Chase event to State Tree */
	void Chase() const;

	/** Turns off widget visibility */
	void HideWidget() const;

protected:
	
	/** Path for the enemy to follow (or guard location) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APatrolPath* PatrolPath;

	/** Current index in patrol path */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PatrolIndex;

	/** Enemy sees player or remembers seeing them */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanSeePlayer;

	/** Used for flee state */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bShouldRetreat;

	/** Used when seeing a corpse */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsPermanentlyAlert;

	/** For investigation */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector TargetLocation;

	/** For chasing / attacking */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> AggroList;

	/** Current health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	/** Direction of patrol. Only really useful when patrol doesn't cycle.
	True - Forwards, False - Backwards */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PatrolDirection;

	/** Death animation montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* DeathMontage;

	/** Alert Bark */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* AlertSound;

	/** State Tree Logic */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStateTreeComponent* StateTreeComponent;

	/** AI Perception Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAIPerceptionComponent* PerceptionComponent;

	/** AI Perception Stimuli Source Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;

	/** Widget Component for Exclamation Mark */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UWidgetComponent* WidgetComponent;

public:

	bool IsDead() const { return !bIsAlive; }

	bool CanSeePlayer() const { return bCanSeePlayer; }

	/** Returns the patrol path */
	APatrolPath* GetPatrolPath() const { return PatrolPath; }

	/** Returns the patrol index */
	int32 GetPatrolIndex() const { return PatrolIndex; }

	/** Updates patrol index to the next point in sequence */
	void UpdatePatrolIndex();
	
};
