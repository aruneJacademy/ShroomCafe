// Fill out your copyright notice in the Description page of Project Settings.


#include "TileSpawner.h"
#include "Components/CapsuleComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Tile.h"
#include "WFCManager.h"
#include "WFCUtils.h"
#include "TileSpawnRule.h"

// Sets default values
ATileSpawner::ATileSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AActor> BPActor(TEXT("/Script/Engine.Blueprint'/Game/WFC/BP_Rock.BP_Rock_C'"));
	if (BPActor.Succeeded())
	{
		BPClassRockTile = BPActor.Class;
	}

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

void ATileSpawner::SpawnTile(AWFCManager* Manager, FCell* Cell, uint8 TileID)
{
	ATile* Tile = WFCUtils::SpawnTile(Manager->GetWorld(), TileID);

	TileSpawnRule SpawnRule;

	if (Tile)
	{
		Tile->SetActorLocation(Cell->GetWorldPos());
		SpawnRule.SetMesh(Tile, &AWFCManager::Tiles[ TileID ], TileID);
		SpawnRule.SetCapsule(Tile, TileID);
	}
}



