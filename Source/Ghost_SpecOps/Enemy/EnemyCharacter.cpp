// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "../Tasks/PatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/StateTreeComponent.h"
#include "Ghost_SpecOps/Civilian/CivilianCharacter.h"
#include "Ghost_SpecOps/Player/PlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

AEnemyCharacter::AEnemyCharacter() :
	PatrolIndex(0),
	bCanSeePlayer(false),
	bIsDead(false),
	bShouldRetreat(false),
	bIsAlert(false),
	Health(100.f),
	PatrolDirection(true)
{
	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("State Tree"));
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));

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
		
		StimuliSourceComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
	}
	else if(Health <= 20)
	{
		bShouldRetreat = true;
		FGameplayTag RetreatTag = RetreatTag.RequestGameplayTag("Retreat");
		const FStateTreeEvent RetreatEvent;
		StateTreeComponent->SendStateTreeEvent(RetreatEvent);
	}
	
}

void AEnemyCharacter::ProcessStimuli(AActor* Actor, FAIStimulus Stimulus)
{
	//Handle Vision
	if(Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		// Alert on seeing corpse
		if(Cast<AEnemyCharacter>(Actor) || Cast<ACivilianCharacter>(Actor))
		{
			bIsAlert = true;
			FGameplayTag Tag = Tag.RequestGameplayTag("Alert");
			StateTreeComponent->SendStateTreeEvent(FStateTreeEvent(Tag));
		}
		// Chase
		else if(Cast<APlayerCharacter>(Actor))
		{
			bCanSeePlayer = Stimulus.IsActive();
			if(Stimulus.IsActive())
			{
				TargetLocation = Stimulus.StimulusLocation;
				FGameplayTag Tag = Tag.RequestGameplayTag("Chase");
				StateTreeComponent->SendStateTreeEvent(FStateTreeEvent(Tag));	
			}
		}
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
