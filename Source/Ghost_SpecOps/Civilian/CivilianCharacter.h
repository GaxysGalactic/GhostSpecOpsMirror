// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter/BaseCharacter.h"
#include "GameplayTagContainer.h"
#include "Perception/AIPerceptionTypes.h"
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

private:

	virtual void BeginPlay() override;

	/** Switches the "desire" of the civilian to use a computer on their next State Tree runthrough, based on a timer */
	void SwitchSchedule();

	/** Starts the State Tree Logic */
	void StartStateTree() const;

	/** Taking damage */
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	/** Process incoming Perception Stimuli */
	UFUNCTION()
	void ProcessStimuli(AActor* Actor, FAIStimulus Stimulus);

protected:

	/** Used for the "Cower" state / animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFrightened;

	/** Indicates if being alive is essential to mission success. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEssential;
	

	/** Indicates if the civilian should try to use a computer at their earliest possible convenience */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bUseComputer;
	
	/** Timer for switching between computer / roam desires */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTimerHandle ScheduleTimer;

	/** Schedule timer change rate. This will pause when interacting with SO. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScheduleTimerRate;

	/** Radius for their randomized movement in roam state*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveRadius;

	/** Radius to outline search box for finding smart objects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SearchRadius;

	/** Death animation montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage* DeathMontage;

	/** Gameplay tag container for activities civilian can use related to Smart Objects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmartObjects)
	FGameplayTagContainer SmartObjectActivities;
	
	/** State Tree Logic */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStateTreeComponent* StateTreeComponent;

	/** AI Perception Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAIPerceptionComponent* PerceptionComponent;

	/** AI Perception Stimuli Source Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;

public:

	bool IsDead() const { return !bIsAlive; }
	
};
