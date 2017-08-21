// Fill out your copyright notice in the Description page of Project Settings.

#include "Lamp.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ALamp::ALamp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh and set as the root component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Game/StarterContent/Props/SM_Lamp_Ceiling.SM_Lamp_Ceiling"));

	if (Asset.Succeeded())
	{
		Mesh->SetStaticMesh(Asset.Object);
	}

	// Create spot light component and attach to the mesh
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->Intensity = 85000;
	SpotLight->SetLightColor(FLinearColor::Red);
	SpotLight->bVisible = false;
	SpotLight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SpotLight->SetRelativeLocation(FVector(0.0f, 0.0f, -130.0f));
	SpotLight->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void ALamp::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ALamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If lamp is turn on change colors
	if (SpotLight->bVisible)
	{
		ChangeColors(DeltaTime);
	}	
}

// Toggle light
void ALamp::ToggleLight()
{
	SpotLight->ToggleVisibility();
}

void ALamp::ChangeColors(float dt)
{
	TimeForChangingColor -= dt;

	if (TimeForChangingColor < 0)
	{
		if (SpotLight->GetLightColor() == FLinearColor::Red)
		{
			SpotLight->SetLightColor(FLinearColor::Green);
		}
		else if (SpotLight->GetLightColor() == FLinearColor::Green)
		{
			SpotLight->SetLightColor(FLinearColor::Blue);
		}
		else if (SpotLight->GetLightColor() == FLinearColor::Blue)
		{
			SpotLight->SetLightColor(FLinearColor::Red);
		}

		TimeForChangingColor = 1.0f;
	}
}