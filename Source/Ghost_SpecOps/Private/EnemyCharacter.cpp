// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "PatrolPath.h"
#include "Components/SplineComponent.h"

AEnemyCharacter::AEnemyCharacter() :
	PatrolIndex(0),
	CanSeePlayer(false),
	Health(100.f)
{
}

void AEnemyCharacter::UpdatePatrolIndex()
{
	USplineComponent* Path = PatrolPath->GetPath();
	if(Path)
	{
		// If it's a single point...
		if(Path->GetNumberOfSplinePoints() == 1)
		{
			return;
		}
		
		// If it's a closed loop...
		if(Path->IsClosedLoop())
		{
			if(PatrolIndex + 1 >= Path->GetNumberOfSplinePoints())
			{
				PatrolIndex = 0;
			}
			else
			{
				++PatrolIndex;
			}
		}
		// If it's not...
		else
		{
			//TODO
		}
		
	}
}
