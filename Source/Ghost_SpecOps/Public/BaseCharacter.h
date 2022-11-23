// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Weapon.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class AWeapon;

UCLASS()
class GHOST_SPECOPS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> StartingWeaponClass;

	UPROPERTY(EditDefaultsOnly)
	FName WeaponSocketName;
	
	UPROPERTY(BlueprintReadOnly)
	AWeapon* CurrentWeapon;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCrouchButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bJumpButton;


public:	
	virtual void Tick(float DeltaTime) override;

};
