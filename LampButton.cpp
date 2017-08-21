// Fill out your copyright notice in the Description page of Project Settings.

#include "LampButton.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ALampButton::ALampButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Keep movment synced from server to clients
	bReplicates = true;
	bReplicateMovement = true;

	// Create sphere component for checking if player is near the button
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(50.0f);
	Sphere->bGenerateOverlapEvents = true;
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ALampButton::OnOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ALampButton::OnOverlapEnd);
	RootComponent = Sphere;

	// Create mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("/Game/FirstPerson/Meshes/BaseMaterial.BaseMaterial"));
	if (Asset.Succeeded() && Material.Succeeded())
	{
		Mesh->SetStaticMesh(Asset.Object);
		Mesh->SetMaterial(0, Material.Object);
		Mesh->SetWorldScale3D(FVector(0.1f, 0.2f, 0.2f));
	}
	Mesh->SetupAttachment(RootComponent);
}

void ALampButton::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	;
}

// Called when the game starts or when spawned
void ALampButton::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ALampButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// If player is near the button set isPlayerNearTheButton state as true
void ALampButton::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bPlayerNearTheButton = true;
}

// If player leave near the button area set isPlayerNearTheButton state as false
void ALampButton::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	bPlayerNearTheButton = false;
}

void ALampButton::Used()
{
	if (Role == ROLE_Authority)
	{
		ClientUsed();
	}
}

void ALampButton::ClientUsed_Implementation()
{
	if (LampTarget)
	{
		LampTarget->ToggleLight();
	}
		
	if (Mesh->GetComponentScale().X == 0.1f)
	{
		Mesh->SetWorldScale3D(FVector(0.05f, 0.2f, 0.2f));
	}
	else
	{
		Mesh->SetWorldScale3D(FVector(0.1f, 0.2f, 0.2f));
	}
}