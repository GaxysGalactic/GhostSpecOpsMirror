// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "PatrolPath.h"
#include "Components/SplineComponent.h"

AEnemyCharacter::AEnemyCharacter() :
	PatrolIndex(0),
	CanSeePlayer(false),
	Health(100.f),
	PatrolDirection(true)
{
}

void AEnemyCharacter::UpdatePatrolIndex()
{
	if(const USplineComponent* Path = PatrolPath->GetPath())
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
			if(PatrolDirection)
			{
				if(PatrolIndex + 1 >= Path->GetNumberOfSplinePoints())
				{
					PatrolDirection = false;
					--PatrolIndex;
				}
				else
				{
					++PatrolIndex;
				}
			}
			else
			{
				if(PatrolIndex - 1 < 0)
				{
					PatrolDirection = true;
					++PatrolIndex;
				}
				else
				{
					--PatrolIndex;
				}
			}
		}
		
	}
}
