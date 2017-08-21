// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/DestructibleComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/DestructibleMesh.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "EngineGlobals.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "ShooterProjectile.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "DestructibleBox.generated.h"

UCLASS()
class SHOOTER_API ADestructibleBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Required network setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Destroy state
	bool bDestroyed = false;

	// Initial explosion state
	UPROPERTY(Replicated)
	bool bInitialExplosion = false;

public:
	// Destructible component
	UPROPERTY(EditAnywhere)
	class UDestructibleComponent* DestructibleComponent;

	// Destructible mesh
	UPROPERTY(EditAnywhere)
	class UDestructibleMesh* DestructibleMesh;

	// Box component for collision
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	// Particle system component showing explosion
	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* ExplosionParticle;

	// Explosion sound
	UPROPERTY(EditAnywhere)
	class USoundCue* ExplosionSound;

	UPROPERTY(EditAnywhere)
	class UAudioComponent* AudioComponent;

	// Base damage
	UPROPERTY(EditAnywhere)
	float BaseDamage = 10.f;

	// Damage radius
	UPROPERTY(EditAnywhere)
	float DamageRadius = 100.f;
	
	// Impulse strength
	UPROPERTY(EditAnywhere)
	float ImpulseStrength = 12500.f;

	// Time to destroy object
	UPROPERTY(EditAnywhere)
	float ExistingTime = 3.f;

	// Initial explosion
	UFUNCTION()
	void InitialExplosion();

	// Called when projectile hits box
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
