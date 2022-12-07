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

	UFUNCTION(BlueprintCallable)
	void StartFire();
	
protected:
	
	UPROPERTY(BlueprintReadOnly, Replicated, VisibleAnywhere)
	bool bIsAiming;

	UPROPERTY(BlueprintReadOnly, Replicated, VisibleAnywhere)
	bool bIsRunning;

	UPROPERTY(BlueprintReadWrite, Replicated, VisibleAnywhere)
	bool bIsProne;

	UPROPERTY(BlueprintReadWrite, Replicated, VisibleAnywhere)
	bool bIsStanding;

	//AimOffset
	UPROPERTY(BlueprintReadOnly)
	float AO_Pitch;
	
	UPROPERTY(BlueprintReadOnly)
	float AO_Yaw;
	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
