// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToPoint.h"

#include "AIController.h"

EStateTreeRunStatus UMoveToPoint::EnterState(FStateTreeExecutionContext& Context,
                                             const FStateTreeTransitionResult& Transition)
{
	if(Actor)
	{
		APawn* Pawn = Cast<APawn>(Actor);
		if(Pawn)
		{
			AAIController* Controller = Cast<AAIController>(Pawn->GetController());
			if(Controller)
			{
				const FPathFollowingRequestResult RequestResult = Controller->MoveTo(TargetLocation);

				Controller->ReceiveMoveCompleted.AddDynamic(this, &UMoveToPoint::FinishMovement);
			}
		}
	}
	return EStateTreeRunStatus::Running;
}

void UMoveToPoint::FinishMovement(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	bIsMoving = false;
}

EStateTreeRunStatus UMoveToPoint::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if(bIsMoving)
	{
		return EStateTreeRunStatus::Running;
	}
	return EStateTreeRunStatus::Succeeded;
}

void UMoveToPoint::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if(Actor)
	{
		APawn* Pawn = Cast<APawn>(Actor);
		if(Pawn)
		{
			AAIController* Controller = Cast<AAIController>(Pawn->GetController());
			if(Controller)
			{
				Controller->ReceiveMoveCompleted.RemoveDynamic(this, &UMoveToPoint::FinishMovement);
			}
		}
	}
	
	Super::ExitState(Context, Transition);
}


