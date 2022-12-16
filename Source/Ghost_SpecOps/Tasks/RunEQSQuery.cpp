// Fill out your copyright notice in the Description page of Project Settings.


#include "RunEQSQuery.h"

#include "EnvironmentQuery/EnvQueryManager.h"

EStateTreeRunStatus URunEQSQuery::EnterState(FStateTreeExecutionContext& Context,
                                             const FStateTreeTransitionResult& Transition)
{
	FEnvQueryRequest Request(Query,Actor);
	
	Request.Execute(RunMode, this, &URunEQSQuery::FinishQuery);
	
	return Super::EnterState(Context, Transition);
}

void URunEQSQuery::FinishQuery(TSharedPtr<FEnvQueryResult> ResultPtr)
{
	// Do a Target location variable in enemy, make this a separate state


	// OR add a "WaitBeforeMoving" to MoveTo...
	FEnvQueryResult* Result = ResultPtr.Get();

	if(Result)
	{
		TargetLocation = Result->GetItemAsLocation(0);
	}
}
