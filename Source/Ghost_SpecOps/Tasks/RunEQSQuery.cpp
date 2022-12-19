// Fill out your copyright notice in the Description page of Project Settings.


#include "RunEQSQuery.h"

#include "EnvironmentQuery/EnvQueryManager.h"

EStateTreeRunStatus URunEQSQuery::EnterState(FStateTreeExecutionContext& Context,
                                             const FStateTreeTransitionResult& Transition)
{
	FEnvQueryRequest Request(Query,Actor);
	
	Request.Execute(RunMode, this, &URunEQSQuery::FinishQuery);
	bIsFinished = false;
	
	return Super::EnterState(Context, Transition);
}

void URunEQSQuery::FinishQuery(TSharedPtr<FEnvQueryResult> ResultPtr)
{
	FEnvQueryResult* Result = ResultPtr.Get();

	if(Result)
	{
		TargetLocation = Result->GetItemAsLocation(0);
		bIsFinished = true;
	}
}
