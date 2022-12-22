#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsBottom;
	UTexture2D* CrosshairsLeft;

	float CrosshairSpread;
};

UCLASS()
class GHOST_SPECOPS_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	FORCEINLINE void SetHudPackage(const FHUDPackage& InPackage) { HUDPackage = InPackage; }

private:
	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* InTexture, FVector2d InViewportCenter, FVector2D InStread);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;
	
};
