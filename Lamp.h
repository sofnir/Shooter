// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SpotLightComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h" 
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Lamp.generated.h"

UCLASS()
class SHOOTER_API ALamp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALamp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Mesh for the lamp
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;

	// Spot light component
	UPROPERTY(EditAnywhere)
	class USpotLightComponent* SpotLight;

	// Time for changing colors
	UPROPERTY(EditAnywhere)
	float TimeForChangingColor = 1.f;

	// Toggle light visibility 
	UFUNCTION()
	void ToggleLight();

	// Change light colors
	UFUNCTION()
	void ChangeColors(float dt);
};
