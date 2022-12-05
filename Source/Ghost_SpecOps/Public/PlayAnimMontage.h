// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "PlayAnimMontage.generated.h"

/**
 * 
 */
UCLASS()
class GHOST_SPECOPS_API UPlayAnimMontage : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	void EndState();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Context")
	AActor* Actor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	class UAnimMontage* AnimMontage;
	
	FTimerHandle AnimationTimer;

	bool bIsRunning;
	
};
