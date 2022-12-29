// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "AIController.h"
#include "../Tasks/PatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/StateTreeComponent.h"
#include "Components/WidgetComponent.h"
#include "Ghost_SpecOps/Civilian/CivilianCharacter.h"
#include "Ghost_SpecOps/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Sound/SoundCue.h"

AEnemyCharacter::AEnemyCharacter() :
	PatrolIndex(0),
	bCanSeePlayer(false),
	bShouldRetreat(false),
	bIsPermanentlyAlert(false),
	Health(100.f),
	PatrolDirection(true)
{
	// Components
	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("State Tree"));
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetVisibility(false);

	// Delegate Binding
	OnTakeAnyDamage.AddDynamic(this, &AEnemyCharacter::TakeDamage);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::ProcessStimuli);
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
	// Death
	if (Health <= 0)
	{
		bIsAlive = false;
		FGameplayTag DeathTag = DeathTag.RequestGameplayTag("Dead");
		const FStateTreeEvent DeathEvent(DeathTag);
		StateTreeComponent->SendStateTreeEvent(DeathEvent);
	}
	// Retreat
	else if(Health <= 20)
	{
		bShouldRetreat = true;
		FGameplayTag RetreatTag = RetreatTag.RequestGameplayTag("Retreat");
		const FStateTreeEvent RetreatEvent(RetreatTag);
		StateTreeComponent->SendStateTreeEvent(RetreatEvent);
	}
	
}

void AEnemyCharacter::ProcessVision(AActor* Actor, FAIStimulus Stimulus)
{
	// Permanently Alert on seeing corpse
	if(Stimulus.IsActive() && !bIsPermanentlyAlert && (Actor->IsA<AEnemyCharacter>() || Actor->IsA<ACivilianCharacter>()))
	{
		if(Actor->IsA<AEnemyCharacter>())
		{
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor);
			if(Enemy->IsDead())
			{
				bIsPermanentlyAlert = true;
				TargetLocation = Stimulus.StimulusLocation;
				Alert();
			}
		}
		else
		{
			ACivilianCharacter* Civilian = Cast<ACivilianCharacter>(Actor);
			if(Civilian->IsDead())
			{
				bIsPermanentlyAlert = true;
				TargetLocation = Stimulus.StimulusLocation;
				Alert();
			}
		}
	}
	// Player-sourced stimuli
	else if(Actor->IsA<APlayerCharacter>())
	{
		// Player enters vision
		if(Stimulus.IsActive())
		{
			const bool bFirstAggro = AggroList.IsEmpty();
			AggroList.Add(Actor);
			if(bFirstAggro)
			{
				Chase();
			}
		}
		// Player exits vision
		else if(!Stimulus.IsActive())
		{
			AggroList.Remove(Actor);

			// If vision lost on all players, investigate around area
			if(AggroList.IsEmpty())
			{
				TargetLocation = Stimulus.StimulusLocation;
				Alert();
			}
		}

		if(!AggroList.IsEmpty())
		{
			AAIController* AIController = Cast<AAIController>(GetController());
			if(AIController)
			{
				AIController->SetFocus(AggroList[0]);
			}
		}
		else
		{
			AAIController* AIController = Cast<AAIController>(GetController());
			if(AIController)
			{
				AIController->ClearFocus(EAIFocusPriority::Gameplay);
			}
		}
		
		// Update CanSeePlayer variable
		bCanSeePlayer = !AggroList.IsEmpty();
		if(bCanSeePlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("CanSeePlayer"));
		}
	}
}

void AEnemyCharacter::ProcessStimuli(AActor* Actor, FAIStimulus Stimulus)
{
	// Handle Vision
	if(Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		ProcessVision(Actor, Stimulus);
	}
	// Handle Hearing
	else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		// Investigate
		if(Stimulus.IsActive() && !bCanSeePlayer)
		{
			TargetLocation = Stimulus.StimulusLocation;
			Alert();
		}
	}
}

void AEnemyCharacter::Alert() const
{
	const FGameplayTag Tag = Tag.RequestGameplayTag("Alert");
	StateTreeComponent->SendStateTreeEvent(FStateTreeEvent(Tag));
}

void AEnemyCharacter::Chase() const
{
	UGameplayStatics::PlaySoundAtLocation(this, AlertSound, GetActorLocation(), GetActorRotation());
	WidgetComponent->SetVisibility(true);

	FTimerHandle TurnOffWidgetHandle;
	GetWorldTimerManager().SetTimer(TurnOffWidgetHandle, this, &AEnemyCharacter::HideWidget, 2.f, false);
	
	const FGameplayTag Tag = Tag.RequestGameplayTag("Chase");
	StateTreeComponent->SendStateTreeEvent(FStateTreeEvent(Tag));	
}

void AEnemyCharacter::HideWidget() const
{
	WidgetComponent->SetVisibility(false);
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
