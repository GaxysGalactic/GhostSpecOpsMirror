// Fill out your copyright notice in the Description page of Project Settings.


#include "FindSmartObject.h"
#include "SmartObjectSubsystem.h"

EStateTreeRunStatus UFindSmartObject::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	bShouldStateChangeOnReselect = false;
		
	FBox SearchArea(Actor->GetActorLocation() - SearchRadius, Actor->GetActorLocation() + SearchRadius);
	
	FSmartObjectRequestFilter Filter;

	FGameplayTagQueryExpression Expression;
	Expression.AnyTagsMatch();
	Expression.AddTag(SmartObjectTag);
	
	Filter.ActivityRequirements.Build(Expression);
	
	FSmartObjectRequest Request(SearchArea, Filter);
	
	FSmartObjectRequestResult Result = GetWorld()->GetSubsystem<USmartObjectSubsystem>()->FindSmartObject(Request);

	if(Result.IsValid())
	{
		SmartObjectClaimHandle = GetWorld()->GetSubsystem<USmartObjectSubsystem>()->Claim(
		Result.SmartObjectHandle, Result.SlotHandle);

		SmartObjectLocation = GetWorld()->GetSubsystem<USmartObjectSubsystem>()->GetSlotLocation(SmartObjectClaimHandle).GetValue();
	
		UE_LOG(LogTemp, Warning, TEXT("Claim success"));
		return EStateTreeRunStatus::Running;
	}
	UE_LOG(LogTemp, Warning, TEXT("Claim failed."));
	return EStateTreeRunStatus::Failed;
}

void UFindSmartObject::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if(SmartObjectClaimHandle.IsValid())
	{
		//GetWorld()->GetSubsystem<USmartObjectSubsystem>()->Release(SmartObjectClaimHandle);
	}
	Super::ExitState(Context, Transition);
}
