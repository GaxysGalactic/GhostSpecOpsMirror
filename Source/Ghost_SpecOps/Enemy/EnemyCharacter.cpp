// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "../Tasks/PatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/StateTreeComponent.h"

AEnemyCharacter::AEnemyCharacter() :
	PatrolIndex(0),
	CanSeePlayer(false),
	bIsDead(false),
	bShouldRetreat(false),
	Health(100.f),
	PatrolDirection(true)
{
	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("State Tree"));

	OnTakeAnyDamage.AddDynamic(this, &AEnemyCharacter::TakeDamage);
}

void AEnemyCharacter::BeginPlay()
{
	if(HasAuthority())
	{
		FTimerHandle StartLogicHandle;
		GetWorldTimerManager().SetTimer(StartLogicHandle, this, &AEnemyCharacter::StartStateTree, 5.f, false);
	}
	Super::BeginPlay();
}

void AEnemyCharacter::StartStateTree() const
{
	StateTreeComponent->StartLogic();
}

void AEnemyCharacter::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                 AController* InstigatedBy, AActor* DamageCauser)
{
	Health -= Damage;
	if (Health <= 0)
	{
		bIsDead = true;
		FGameplayTag DeathTag = DeathTag.RequestGameplayTag("Dead");
		const FStateTreeEvent DeathEvent;
		StateTreeComponent->SendStateTreeEvent(DeathEvent);
	}
	else if(Health <= 20)
	{
		bShouldRetreat = true;
		FGameplayTag RetreatTag = RetreatTag.RequestGameplayTag("Retreat");
		const FStateTreeEvent RetreatEvent;
		StateTreeComponent->SendStateTreeEvent(RetreatEvent);
	}
	
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
