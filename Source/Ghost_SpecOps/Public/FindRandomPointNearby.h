// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "FindRandomPointNearby.generated.h"

/**
 * 
 */
UCLASS()
class GHOST_SPECOPS_API UFindRandomPointNearby : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;


protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Context")
	AActor* Actor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	float Radius;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Output")
	FVector RandomLocation;
	
};
