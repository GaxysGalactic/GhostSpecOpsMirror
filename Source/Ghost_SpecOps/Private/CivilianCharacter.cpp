// Fill out your copyright notice in the Description page of Project Settings.


#include "CivilianCharacter.h"

ACivilianCharacter::ACivilianCharacter() :
	bIsFrightened(false),
	bIsEssential(false),
	bIsDead(false),
	bUseComputer(false)
{
	
}

void ACivilianCharacter::BeginPlay()
{
	GetWorldTimerManager().SetTimer(ScheduleTimer, this, &ACivilianCharacter::SwitchSchedule, 30.f, true);
	Super::BeginPlay();
}

void ACivilianCharacter::SwitchSchedule()
{
	bUseComputer = !bUseComputer;
}
