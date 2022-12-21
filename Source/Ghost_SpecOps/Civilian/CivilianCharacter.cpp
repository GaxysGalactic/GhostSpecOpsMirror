// Fill out your copyright notice in the Description page of Project Settings.


#include "CivilianCharacter.h"

ACivilianCharacter::ACivilianCharacter() :
	bIsFrightened(false),
	bIsEssential(false),
	bIsDead(false),
	bUseComputer(true),
	ScheduleTimerRate(15.f),
	MoveRadius(1000.f),
	SearchRadius(3000.f)
{
	
}

void ACivilianCharacter::BeginPlay()
{
	GetWorldTimerManager().SetTimer(ScheduleTimer, this, &ACivilianCharacter::SwitchSchedule, ScheduleTimerRate, true);
	Super::BeginPlay();
}

void ACivilianCharacter::SwitchSchedule()
{
	bUseComputer = !bUseComputer;
}
