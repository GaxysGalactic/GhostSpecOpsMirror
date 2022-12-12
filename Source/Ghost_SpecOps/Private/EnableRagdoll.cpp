// Fill out your copyright notice in the Description page of Project Settings.


#include "EnableRagdoll.h"

#include "GameFramework/Character.h"

void UEnableRagdoll::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	ACharacter* Character = Cast<ACharacter>(Actor);
	if(Character)
	{
		Character->GetMesh()->SetCollisionProfileName("Ragdoll");
		Character->GetMesh()->SetAllBodiesSimulatePhysics(true);
	}
	Super::ExitState(Context, Transition);
}
