// Fill out your copyright notice in the Description page of Project Settings.


#include "WFCUtils.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/StaticMeshActor.h>
#include "WFCManager.h"
#include "TileSpawner.h"

//UClass* WFCUtils::BPClassRockTile{ nullptr };

WFCUtils::WFCUtils()
{

}

WFCUtils::~WFCUtils()
{

}

UStaticMeshComponent* WFCUtils::GetTileMesh(FString TargetName, UWorld* World)
{
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

ATile* WFCUtils::SpawnTile(UWorld* World, uint8 TileID)
{
	ATile* NewTile{ nullptr };

	if (TileID == static_cast< uint8 >(ETileType::Path))
	{
		AWFCManager* Manager = Cast<AWFCManager>(UGameplayStatics::GetActorOfClass(World, AWFCManager::StaticClass()));
		ATileSpawner* TileSpawner = Cast<ATileSpawner>(UGameplayStatics::GetActorOfClass(World, ATileSpawner::StaticClass()));

		// Ensure the class is valid before spawning
		if (TileSpawner->GetRockTile())
		{
			// Spawn the actor
			AActor* SpawnedActor = World->SpawnActor<AActor>(TileSpawner->GetRockTile());
			if (SpawnedActor)
			{
				NewTile = Cast<ATile>(SpawnedActor);
			}
		}
	}
	else
	{
		FActorSpawnParameters SpawnParams;
		NewTile = World->SpawnActor< ATile >(ATile::StaticClass());
	}
	return NewTile;
}


void WFCUtils::SortCellsByDistance(FCell* Target, TArray< FCell* >& Cells)
{
	TArray < float > Distances;

	for (int i = 0; i < Cells.Num(); i++)
	{
		FCell* Cell = Cells[ i ];
		FVector TargetPos = Target->GetWorldPos();
		FVector CellPos = Cell->GetWorldPos();

		float Distance = FVector::Dist(TargetPos, CellPos);
		Distances.Add(Distance);
	}

	// Create an array of indices
	TArray<int32> SortedIndices;
	SortedIndices.SetNum(Distances.Num());
	for (int i = 0; i < Distances.Num(); ++i)
	{
		SortedIndices[ i ] = i;
	}

	// Sort the indices based on the Distances array
	SortedIndices.Sort([ &Distances ](int32 A, int32 B) {
		return Distances[ A ] < Distances[ B ];
		});

	// Reorder the Cells array based on the sorted indices
	TArray< FCell* >SortedCells;

	for (int i = 0; i < SortedIndices.Num(); ++i)
	{
		SortedCells.Add(Cells[ SortedIndices[ i ] ]);
	}

	Cells = SortedCells;
}

bool WFCUtils::IsPositionWithinBounds(FIntPoint* Pos, AWFCGrid* Grid)
{
	return (Pos->X >= 0 && Pos->Y >= 0 && Pos->X < Grid->GetGridWidth() && Pos->Y < Grid->GetGridHeight());
}