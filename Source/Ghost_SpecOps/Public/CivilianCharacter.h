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

private:

	virtual void BeginPlay() override;

	/** Switches the "desire" of the civilian to use a computer on their next State Tree runthrough, based on a timer */
	void SwitchSchedule();

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

	/** Indicates if the civilian should try to use a computer at their earliest possible convenience */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseComputer;
	
	/** Timer for switching between computer / roam desires */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTimerHandle ScheduleTimer;
	
};
