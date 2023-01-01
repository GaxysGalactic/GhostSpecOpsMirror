// Fill out your copyright notice in the Description page of Project Settings.


#include "FireWeapon.h"

#include "StateTreeExecutionContext.h"
#include "Ghost_SpecOps/Components/EnemyCombatComponent.h"
#include "Ghost_SpecOps/Enemy/EnemyCharacter.h"

EStateTreeRunStatus UFireWeapon::EnterState(FStateTreeExecutionContext& Context,
                                            const FStateTreeTransitionResult& Transition)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Actor);
	if(EnemyCharacter)
	{
		EnemyCharacter->GetWorldTimerManager().SetTimer(FireTimer, this, &UFireWeapon::ChangeFire, 1.5f, true);
	}
	return Super::EnterState(Context, Transition);
}

EStateTreeRunStatus UFireWeapon::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Actor);
	if(EnemyCharacter && EnemyCharacter->HasAggro())
	{
		EnemyCharacter->GetEnemyCombatComponent()->OrderToFire(bIsFiring);
	}
	
	return Super::Tick(Context, DeltaTime);
}

void UFireWeapon::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Actor);
	if(Context.GetStateFromHandle(Transition.TargetState)->Name == "Death" ||
		Context.GetStateFromHandle(Transition.TargetState)->Name == "Retreat" )
	{
		EnemyCharacter->GetWorldTimerManager().ClearTimer(FireTimer);
		EnemyCharacter->GetEnemyCombatComponent()->OrderToFire(false);
	}
	Super::ExitState(Context, Transition);
}

void UFireWeapon::ChangeFire()
{
	bIsFiring = !bIsFiring;
}
