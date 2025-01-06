// Fill out your copyright notice in the Description page of Project Settings.


#include "TileSpawner.h"
#include "Components/CapsuleComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Tile.h"
#include "WFCManager.h"
#include "WFCUtils.h"

// Sets default values
ATileSpawner::ATileSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATileSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATileSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileSpawner::SpawnPath(AWFCManager* Manager, FCell* Cell, uint8 TileID)
{
	ATile* Tile = WFCUtils::SpawnTile(Manager->GetWorld(), TileID);

	if (Tile)
	{
		Tile->SetActorLocation(Cell->GetWorldPos());
		UStaticMesh* MeshAsset = LoadObject< UStaticMesh >(nullptr, Manager->GetMeshString(TileID));

		if (MeshAsset)
		{
			UStaticMeshComponent* MeshComp = Tile->FindComponentByClass< UStaticMeshComponent >();
			MeshComp->SetStaticMesh(MeshAsset);

			FVector NewScale(1.5f, 1.5f, 1.5f);
			MeshComp->SetWorldScale3D(NewScale);
			MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ATileSpawner::SpawnTree(AWFCManager* Manager, FCell* Cell, uint8 TileID)
{
	ATile* Tile = WFCUtils::SpawnTile(Manager->GetWorld(), TileID);

	if (Tile)
	{
		Tile->SetActorLocation(Cell->GetWorldPos());
		const TCHAR* MeshString = Manager->GetMeshString(TileID);
		UStaticMesh* MeshAsset = LoadObject< UStaticMesh >(nullptr, MeshString);

		if (MeshAsset)
		{
			UStaticMeshComponent* MeshComp = Tile->FindComponentByClass< UStaticMeshComponent >();
			MeshComp->SetStaticMesh(MeshAsset);

			FVector NewScale(1.5f, 1.5f, 1.5f);
			MeshComp->SetWorldScale3D(NewScale);
			MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		UCapsuleComponent* CapsuleComponent = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass());
		CapsuleComponent->SetupAttachment(Tile->GetRootComponent());  // Attach to the actor's root component or another component
		CapsuleComponent->SetCapsuleHalfHeight(200.0f);  // Set the height of the capsule
		CapsuleComponent->SetCapsuleRadius(70.0f);  // Set the radius of the capsule
		//CapsuleComponent->SetHiddenInGame(false);
		//CapsuleComponent->SetVisibility(true);
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);  // Treats the capsule as a dynamic object
		CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Block);  // Block all interactions
		CapsuleComponent->RegisterComponent();  // Register the component to make it functional
	}
}


void ATileSpawner::SpawnBush(AWFCManager* Manager, FCell* Cell, uint8 TileID)
{
	ATile* Tile = WFCUtils::SpawnTile(Manager->GetWorld(), TileID);

	if (Tile)
	{
		Tile->SetActorLocation(Cell->GetWorldPos());
		UStaticMesh* MeshAsset = LoadObject< UStaticMesh >(nullptr, Manager->GetMeshString(TileID));

		if (MeshAsset)
		{
			UStaticMeshComponent* MeshComp = Tile->FindComponentByClass< UStaticMeshComponent >();
			MeshComp->SetStaticMesh(MeshAsset);

			FVector NewScale(1.5f, 1.5f, 1.5f);
			MeshComp->SetWorldScale3D(NewScale);
			MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ATileSpawner::SpawnGrass(AWFCManager* Manager, FCell* Cell, uint8 TileID)
{
	ATile* Tile = WFCUtils::SpawnTile(Manager->GetWorld(), TileID);

	if (Tile)
	{
		Tile->SetActorLocation(Cell->GetWorldPos());
		UStaticMesh* MeshAsset = LoadObject< UStaticMesh >(nullptr, Manager->GetMeshString(TileID));

		if (MeshAsset)
		{
			UStaticMeshComponent* MeshComp = Tile->FindComponentByClass< UStaticMeshComponent >();
			MeshComp->SetStaticMesh(MeshAsset);

			FVector NewScale(1.5f, 1.5f, 1.5f);
			MeshComp->SetWorldScale3D(NewScale);
			MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}



