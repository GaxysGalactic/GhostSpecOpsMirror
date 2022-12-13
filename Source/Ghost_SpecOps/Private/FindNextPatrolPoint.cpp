// Fill out your copyright notice in the Description page of Project Settings.


#include "FindNextPatrolPoint.h"

#include "EnemyCharacter.h"
#include "PatrolPath.h"

EStateTreeRunStatus UFindNextPatrolPoint::EnterState(FStateTreeExecutionContext& Context,
                                                     const FStateTreeTransitionResult& Transition)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Actor);
	if(EnemyCharacter)
	{
		APatrolPath* PatrolPath = EnemyCharacter->GetPatrolPath();
		if(PatrolPath)
		{
			
		}
	}
	return EStateTreeRunStatus::Failed;
}
