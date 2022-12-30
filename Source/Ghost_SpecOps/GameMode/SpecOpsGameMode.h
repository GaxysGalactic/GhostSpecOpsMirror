// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Ghost_SpecOps/Player/GhostPlayerController.h"
#include "SpecOpsGamemode.generated.h"

class ABaseCharacter;
class AGhostPlayerController;


UCLASS()
class GHOST_SPECOPS_API ASpecOpsGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PlayerEliminated(ABaseCharacter* ElimCharacter, AGhostPlayerController* VictimController, AGhostPlayerController* AttackerController);
	
};
