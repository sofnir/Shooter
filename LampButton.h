// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h" 
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Lamp.h"
#include "LampButton.generated.h"

UCLASS()
class SHOOTER_API ALampButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALampButton();

	// Required network setup
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Is player near the button state
	bool bPlayerNearTheButton = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Mesh for the button
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;

	// Sphere component for checking if player is near the button
	UPROPERTY(EditAnywhere)
	class USphereComponent* Sphere;

	// Lamp pinter for connecting lamp with button
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ALamp *LampTarget;

	// Called when something enters the sphere component - if player is near the button set isPlayerNearTheButton state as true
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when something leaves the sphere component - set isPlayerNearTheButton state as false
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	// Returns isPlayerNearTheButton state 
	UFUNCTION()
	bool getIsPlayerNearTheButton() const { return bPlayerNearTheButton; }

	// Use the button
	UFUNCTION()
	void Used();

	UFUNCTION(NetMulticast, Unreliable)
	void ClientUsed();
};
