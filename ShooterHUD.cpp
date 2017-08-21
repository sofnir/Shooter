// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ShooterHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AShooterHUD::AShooterHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

	// Create font and load my font
	ConstructorHelpers::FObjectFinder<UFont> FontObject(TEXT("/Game/Font/MyFont_Font"));
	if (FontObject.Object)
	{
		Font = FontObject.Object;
	}
}

void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );

	// If player is near the button draw a message 
	class ALampButton *pBest = NULL;

	for (TActorIterator<ALampButton> ButtonItr(GetWorld()); ButtonItr; ++ButtonItr)
	{
		class ALampButton *pButton = *ButtonItr;

		if (pButton->getIsPlayerNearTheButton())
		{
			pBest = pButton;
		}
	}

	if (pBest)
	{
		DrawMessage(Center);
	}
}

void AShooterHUD::DrawMessage(const FVector2D & Position)
{
	FText String = NSLOCTEXT("String", "Key", "Press E for using button.");
	FCanvasTextItem TextItem(FVector2D(Position.X, Position.Y - 200.0f), String, Font, FLinearColor::Green);
	TextItem.bCentreX = true;
	Canvas->DrawItem(TextItem);
}
