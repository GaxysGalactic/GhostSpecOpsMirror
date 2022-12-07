// Fill out your copyright notice in the Description page of Project Settings.


#include "FindSmartObject.h"
#include "SmartObjectSubsystem.h"

EStateTreeRunStatus UFindSmartObject::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{

	FBox SearchArea(Actor->GetActorLocation() + SearchRadius, Actor->GetActorLocation() - SearchRadius);
	
	FSmartObjectRequestFilter Filter;
	Filter.UserTags.AddTag(SmartObjectTag);
	
	FSmartObjectRequest Request(SearchArea, Filter);
	
	FSmartObjectRequestResult Result = GetWorld()->GetSubsystem<USmartObjectSubsystem>()->FindSmartObject(Request);

	if(Result.IsValid())
	{
		SmartObjectClaimHandle = GetWorld()->GetSubsystem<USmartObjectSubsystem>()->Claim(Result.SmartObjectHandle, Result.SlotHandle);
		UE_LOG(LogTemp, Warning, TEXT("Testing"));
		return EStateTreeRunStatus::Running;
	}
	return EStateTreeRunStatus::Failed;
}

void UFindSmartObject::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if(SmartObjectClaimHandle.IsValid())
	{
		GetWorld()->GetSubsystem<USmartObjectSubsystem>()->Release(SmartObjectClaimHandle);
	}
	Super::ExitState(Context, Transition);
}
