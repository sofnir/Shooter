// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleBox.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADestructibleBox::ADestructibleBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Keep movment synced from server to clients
	bReplicates = true;
	bReplicateMovement = true;

	// Create destructible component and  set as root component
	DestructibleComponent = CreateAbstractDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleComponent"));	
	DestructibleComponent->SetWorldScale3D(FVector(0.6f, 0.6f, 0.6f));
	RootComponent = DestructibleComponent;

	// Create destructible mesh and set to the destructible component
	DestructibleMesh = CreateAbstractDefaultSubobject<UDestructibleMesh>(TEXT("DestructibleMesh"));
	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> Asset(TEXT("/Engine/Tutorial/SubEditors/TutorialAssets/Tutorial_DM.Tutorial_DM"));
	DestructibleMesh = Asset.Object;

	if (Asset.Succeeded())
	{
		DestructibleComponent->SetDestructibleMesh(DestructibleMesh);
	}

	// Create box component (collision component) and attach to the destructible component
	BoxComponent = CreateAbstractDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(135.0f, 135.0f, 135.0f));
	BoxComponent->OnComponentHit.AddDynamic(this, &ADestructibleBox::OnHit);
	BoxComponent->BodyInstance.SetCollisionProfileName("DestructibleBox");
	BoxComponent->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics, true);
	BoxComponent->AttachToComponent(DestructibleComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// Create particle system for explosion and attach to the destructible componenet
	ExplosionParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleExplosion"));	
	ExplosionParticle->AttachToComponent(DestructibleComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ExplosionParticle->bAutoActivate = false;
	ExplosionParticle->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	
	if (ParticleAsset.Succeeded())
	{
		ExplosionParticle->SetTemplate(ParticleAsset.Object);
	}

	// Create sound and attach to the destructible component
	static ConstructorHelpers::FObjectFinder<USoundCue> Cue(TEXT("/Game/StarterContent/Audio/Explosion_Cue"));

	if (Cue.Succeeded())
	{
		ExplosionSound = Cue.Object;
	}

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->AttachToComponent(DestructibleComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	AudioComponent->SetSound(ExplosionSound);
	AudioComponent->SetVolumeMultiplier(5.0f);
}

void ADestructibleBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ADestructibleBox, bInitialExplosion);
}

// Called when the game starts or when spawned
void ADestructibleBox::BeginPlay()
{
	Super::BeginPlay();
	
	// Set destructible component collision presets as block state
	DestructibleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

// Called every frame
void ADestructibleBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Checking initial explosion state for initial explosion and destroy state for destroying object after few seconds
	if (bDestroyed)
	{
		ExistingTime -= DeltaTime;

		if (ExistingTime <= 0)
		{
			Destroy();
		}
	}
	else
	{
		if (bInitialExplosion)
		{
			InitialExplosion();			
		}
	}
}

// Initial Explosion
void ADestructibleBox::InitialExplosion()
{
	// Initial force in the center of the box
	FVector Origin;
	FVector BoundsExtent;
	GetActorBounds(false, Origin, BoundsExtent);
	DestructibleComponent->ApplyRadiusDamage(BaseDamage, Origin, DamageRadius, ImpulseStrength, false);

	// Initial explosion particle
	ExplosionParticle->Activate();

	// Play explosion sound
	AudioComponent->Play();

	// Set collision state as ignore and set destroy state as true
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	bDestroyed = true;
}

// Called when projectile hits box
void ADestructibleBox::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{	
	// If projectile hits the box destroy projectile and set initial explosion state as true
	if (OtherActor->IsA(AShooterProjectile::StaticClass()) && !bDestroyed)
	{
		OtherActor->Destroy();
		bInitialExplosion = true;
	}
}