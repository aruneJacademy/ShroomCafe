// Fill out your copyright notice in the Description page of Project Settings.


#include "WFCManager.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/StaticMeshActor.h>

// Sets default values
AWFCManager::AWFCManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWFCManager::BeginPlay()
{
	//Super::BeginPlay();
	
}

// Called every frame
void AWFCManager::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

}

void AWFCManager::CreateTiles()
{
	Tiles.SetNum((int)ETile::Max_Tiles);

	FTileData UnknownTile((int)ETile::Unknown, TArray<uint8>{ (int)ETile::Bush, (int)ETile::Grass, (int)ETile::Tree, (int)ETile::Path, (int)ETile::Empty });
	UnknownTile.SetTileMesh(GetTileMesh(FString( "TileMeshUnknown" )));
	Tiles.Add(UnknownTile);
	
	FTileData GrassTile((int)ETile::Grass, TArray<uint8>{ (int)ETile::Bush, (int)ETile::Grass, (int)ETile::Tree, (int)ETile::Path, (int)ETile::Empty });
	GrassTile.SetTileMesh(GetTileMesh(FString( "TileMeshGrass" )));
	Tiles.Add(GrassTile);

	FTileData TreeTile((int)ETile::Tree, TArray<uint8>{ (int)ETile::Bush, (int)ETile::Grass, (int)ETile::Empty });
	TreeTile.SetTileMesh(GetTileMesh(FString( "TileMeshTree" )));
	Tiles.Add(TreeTile);

	FTileData BushTile((int)ETile::Bush, TArray<uint8>{ (int)ETile::Bush, (int)ETile::Grass, (int)ETile::Tree, (int)ETile::Path });
	BushTile.SetTileMesh(GetTileMesh(FString( "TileMeshBush" )));
	Tiles.Add(BushTile);

	FTileData PathTile((int)ETile::Path, TArray<uint8>{ (int)ETile::Bush, (int)ETile::Grass, (int)ETile::Path });
	PathTile.SetTileMesh(GetTileMesh(FString( "TileMeshPath" )));
	Tiles.Add(PathTile);

	FTileData EmptyTile((int)ETile::Empty, TArray<uint8>{ (int)ETile::Grass, (int)ETile::Tree, (int)ETile::Path, (int)ETile::Empty });
	EmptyTile.SetTileMesh(GetTileMesh(FString( "TileMeshEmpty" )));
	Tiles.Add(EmptyTile);
}

UStaticMeshComponent* AWFCManager::GetTileMesh(FString TargetName)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	// Array to store all actors of the desired class
	TArray<AActor*> FoundActors;

	// Find all actors of the desired class (e.g., AStaticMeshActor)
	UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (Actor && Actor->GetName() == TargetName)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Actor: %s"), *Actor->GetName());

			// Cast to AStaticMeshActor to access the StaticMeshComponent
			AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(Actor);
			if (MeshActor)
			{
				UStaticMeshComponent* MeshComponent = MeshActor->GetStaticMeshComponent();
				if (MeshComponent)
				{
					UE_LOG(LogTemp, Warning, TEXT("Static Mesh Name: %s"), *MeshComponent->GetStaticMesh()->GetName());
					return MeshComponent;
				}
			}
			return nullptr; // Exit the function after finding the actor
		}
	}
	return nullptr;
}
