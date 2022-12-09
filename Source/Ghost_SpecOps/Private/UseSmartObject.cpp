// Fill out your copyright notice in the Description page of Project Settings.


#include "UseSmartObject.h"

#include "FindSmartObject.h"
#include "SmartObjectSubsystem.h"
#include "SmartObjectDefinition.h"

EStateTreeRunStatus UUseSmartObject::EnterState(FStateTreeExecutionContext& Context,
                                                const FStateTreeTransitionResult& Transition)
{
	GetWorld()->GetSubsystem<USmartObjectSubsystem>()->Use<USmartObjectBehaviorDefinition>(SmartObjectClaimHandle);

	
	
	return Super::EnterState(Context, Transition);
}
