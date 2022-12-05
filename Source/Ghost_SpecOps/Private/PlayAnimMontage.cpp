// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayAnimMontage.h"

#include "GameFramework/Character.h"

EStateTreeRunStatus UPlayAnimMontage::EnterState(FStateTreeExecutionContext& Context,
                                                 const FStateTreeTransitionResult& Transition)
{
	ACharacter* Character = Cast<ACharacter>(Actor);
	if(Character)
	{
		float AnimationTime = Character->PlayAnimMontage(AnimMontage);

		Character->GetWorldTimerManager().SetTimer(AnimationTimer, this, &UPlayAnimMontage::EndState, AnimationTime);
		bIsRunning = true;
	}
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus UPlayAnimMontage::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	return bIsRunning ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Succeeded;
}

void UPlayAnimMontage::EndState()
{
	bIsRunning = false;
}
