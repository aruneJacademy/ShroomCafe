// Fill out your copyright notice in the Description page of Project Settings.


#include "WFCManager.h"
#include "WFCUtils.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

// Sets default values
AWFCManager::AWFCManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreateTiles();
}

// Called when the game starts or when spawned
void AWFCManager::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Creating Tiles"));
	
}

// Called every frame
void AWFCManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWFCManager::CreateTiles()
{
	Tiles.SetNum((int)ETileType::Max_Tiles);
	UWorld* World = GetWorld();

	UE_LOG(LogTemp, Warning, TEXT("Creating Tiles Aleliuja 2025"));

	FTile UnknownTile((int)ETileType::Unknown, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Tree, (int)ETileType::Path, (int)ETileType::Empty });
	UnknownTile.SetTileMesh(WFCUtils::GetTileMesh(FString( "TileMeshUnknown" ), World));
	Tiles.Add(UnknownTile);
	UE_LOG(LogTemp, Warning, TEXT("Tile number One: TileMeshUnknown" ));
	
	FTile GrassTile((int)ETileType::Grass, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Tree, (int)ETileType::Path, (int)ETileType::Empty });
	GrassTile.SetTileMesh(WFCUtils::GetTileMesh(FString( "TileMeshGrass" ), World));
	Tiles.Add(GrassTile);
	UE_LOG(LogTemp, Warning, TEXT("Tile number Two: %i"), (int)ETileType::Grass);

	FTile TreeTile((int)ETileType::Tree, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Empty });
	TreeTile.SetTileMesh(WFCUtils::GetTileMesh(FString( "TileMeshTree" ), World));
	Tiles.Add(TreeTile);

	FTile BushTile((int)ETileType::Bush, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Tree, (int)ETileType::Path });
	BushTile.SetTileMesh(WFCUtils::GetTileMesh(FString( "TileMeshBush" ), World));
	Tiles.Add(BushTile);

	FTile PathTile((int)ETileType::Path, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Path });
	PathTile.SetTileMesh(WFCUtils::GetTileMesh(FString( "TileMeshPath" ), World));
	Tiles.Add(PathTile);

	FTile EmptyTile((int)ETileType::Empty, TArray<uint8>{ (int)ETileType::Grass, (int)ETileType::Tree, (int)ETileType::Path, (int)ETileType::Empty });
	EmptyTile.SetTileMesh(WFCUtils::GetTileMesh(FString( "TileMeshEmpty" ), World));
	Tiles.Add(EmptyTile);
}

void AWFCManager::InitializeGrid(int Rows, int Columns)
{
	Grid = GetWorld()->SpawnActor<AWFCGrid>(AWFCGrid::StaticClass());
	Grid->GridHeight = Rows;
	Grid->GridWidth = Columns;

	Grid->Cells.SetNum(Rows);
	for (int Row = 0; Row < Rows; ++Row)
	{
		Grid->Cells[ Row ].SetNum(Columns);  // Create columns for each row
	}

	for (int Row = 0; Row < Rows; ++Row)
	{
		for (int Column = 0; Column < Columns; ++Column)
		{
			//FCell& Cell = Grid->Cells[Row][Column];
			//Cell.WorldPos = FVector2D(Row * 100, Column * 100);  
			// Add all tile IDs to the cell's possible tiles
			for (const FTile& Tile : Tiles)
			{
				//Cell.PossibleTiles.Add(Tile.TileID);
				//UE_LOG(LogTemp, Warning, TEXT("Adding tile to cell at row %i and column %i"), Row, Column);
			}
		}
	}
}

void AWFCManager::GenerateGrid()
{

}

void AWFCManager::SpawnGrid()
{
	//for (int Row = 0; Row < Grid->GridWidth; ++Row)
	{
		//for (int Column = 0; Column < Grid->GridHeight; ++Column)
		{
			//FCell& Cell = Grid->Cells[Row][Column];

			//AStaticMeshActor* TileActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector(Cell.WorldPos.X, Cell.WorldPos.Y, 0), FRotator::ZeroRotator);
			//if (TileActor)
			//{
			//	// Set the static mesh
			//	UStaticMeshComponent* MeshComponent = TileActor->GetStaticMeshComponent();
			//	if (MeshComponent)
			//	{
			//		//MeshComponent->SetStaticMesh(Tiles[0].TileMesh->GetStaticMesh());

			//		// Optional: Configure properties (e.g., collision, material)
			//		//MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
			//		MeshComponent->SetMobility(EComponentMobility::Static);
			//	}
			//}
		}
	}
}

void AWFCManager::OnProceduralGeneration()
{
	UE_LOG(LogTemp, Warning, TEXT("Procedural Generation"));
}