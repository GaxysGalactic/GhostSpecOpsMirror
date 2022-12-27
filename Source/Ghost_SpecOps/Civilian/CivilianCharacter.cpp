// Fill out your copyright notice in the Description page of Project Settings.


#include "CivilianCharacter.h"
#include "Components/StateTreeComponent.h"

ACivilianCharacter::ACivilianCharacter() :
	bIsFrightened(false),
	bIsEssential(false),
	bIsDead(false),
	bUseComputer(true),
	ScheduleTimerRate(15.f),
	MoveRadius(1000.f),
	SearchRadius(3000.f)
{
	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("State Tree"));

	OnTakeAnyDamage.AddDynamic(this, &ACivilianCharacter::TakeDamage);
}

void ACivilianCharacter::BeginPlay()
{
	if(HasAuthority())
	{
		FTimerHandle StartLogicHandle;
		GetWorldTimerManager().SetTimer(StartLogicHandle, this, &ACivilianCharacter::StartStateTree, 5.f, false);
		GetWorldTimerManager().SetTimer(ScheduleTimer, this, &ACivilianCharacter::SwitchSchedule, ScheduleTimerRate, true);
	}
	Super::BeginPlay();
}

void ACivilianCharacter::SwitchSchedule()
{
	bUseComputer = !bUseComputer;
}

void ACivilianCharacter::StartStateTree() const
{
	StateTreeComponent->StartLogic();
}

void ACivilianCharacter::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	bIsDead = true;
	const FGameplayTag Tag = Tag.RequestGameplayTag("Dead");
	const FStateTreeEvent Event(Tag);
	StateTreeComponent->SendStateTreeEvent(Event);
}
