// Fill out your copyright notice in the Description page of Project Settings.


#include "FireWeapon.h"

#include "Ghost_SpecOps/Components/EnemyCombatComponent.h"
#include "Ghost_SpecOps/Enemy/EnemyCharacter.h"

EStateTreeRunStatus UFireWeapon::EnterState(FStateTreeExecutionContext& Context,
                                            const FStateTreeTransitionResult& Transition)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Actor);
	if(EnemyCharacter)
	{
		EnemyCharacter->GetWorldTimerManager().SetTimer(FireTimer, this, &UFireWeapon::ChangeFire, 2.f, true);
	}
	return Super::EnterState(Context, Transition);
}

EStateTreeRunStatus UFireWeapon::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Actor);
	if(EnemyCharacter)
	{
		EnemyCharacter->GetEnemyCombatComponent()->OrderToFire(bIsFiring);
	}
	
	return Super::Tick(Context, DeltaTime);
}

void UFireWeapon::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}

void UFireWeapon::ChangeFire()
{
	bIsFiring = !bIsFiring;
}
