// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LampButton.h"
#include "Runtime/Core/Public/Internationalization/Text.h"
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "ShooterHUD.generated.h"

UCLASS()
class AShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	AShooterHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	// Font
	UPROPERTY(EditAnywhere)
	class UFont *Font;

	UFUNCTION()
	void DrawMessage(const FVector2D & Position);

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
};

