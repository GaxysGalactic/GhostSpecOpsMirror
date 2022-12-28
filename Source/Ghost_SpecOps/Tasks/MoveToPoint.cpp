// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToPoint.h"

#include "AIController.h"
#include "Ghost_SpecOps/Enemy/EnemyCharacter.h"

EStateTreeRunStatus UMoveToPoint::EnterState(FStateTreeExecutionContext& Context,
                                             const FStateTreeTransitionResult& Transition)
{
	if(!bTriggerOnEnterState)
	{
		bIsMoving = false;
		return EStateTreeRunStatus::Running;
	}
	return RequestMovement();
}

void UMoveToPoint::FinishMovement(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	//THE ERROR WAS HERE. IT WAS REQUESTING MOVEMENT AGAIN. MAKE ANOTHER VARIABLE OR STOP IT FROM ENTERING LINE 30
	bIsMoving = false;
}

EStateTreeRunStatus UMoveToPoint::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if(!bTriggerOnEnterState)
	{
		if(bIsPreviousTaskFinished && !bIsMoving)
		{
			return RequestMovement();
		}
		return EStateTreeRunStatus::Running;
	}
	/*
	if(!bIsMoving)
	{
		if(Actor->IsA<AEnemyCharacter>())
		{
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor);
			if(Enemy->CanSeePlayer())
			{
				UE_LOG(LogTemp, Warning, TEXT("State should go to Wait"));
			}
			
			UE_LOG(LogTemp, Warning, TEXT("State should succeed"));
		}
	}
	*/
	return bIsMoving ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Succeeded;
}

void UMoveToPoint::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	APawn* Pawn = Cast<APawn>(Actor);
	if(Pawn)
	{
		AAIController* Controller = Cast<AAIController>(Pawn->GetController());
		if(Controller)
		{
			Controller->ReceiveMoveCompleted.RemoveDynamic(this, &UMoveToPoint::FinishMovement);
			Controller->StopMovement();
		}
	}
	
	Super::ExitState(Context, Transition);
}

EStateTreeRunStatus UMoveToPoint::RequestMovement()
{
	APawn* Pawn = Cast<APawn>(Actor);
	if(Pawn)
	{
		AAIController* Controller = Cast<AAIController>(Pawn->GetController());
		if(Controller)
		{
			const FPathFollowingRequestResult RequestResult = Controller->MoveTo(TargetLocation);
			bIsMoving = true;

			Controller->ReceiveMoveCompleted.AddUniqueDynamic(this, &UMoveToPoint::FinishMovement);

			return EStateTreeRunStatus::Running;
		}
	}
	return EStateTreeRunStatus::Failed;
}


