// Fill out your copyright notice in the Description page of Project Settings.


#include "WFCManager.h"
#include "WFCUtils.h"
#include "DrawDebugHelpers.h"
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
	
}

// Called every frame
void AWFCManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bGenerated)
	{
		OnProceduralGeneration();
		bGenerated = true;
	}

	for (int Row = 0; Row < GridWidth; ++Row)
	{
		for (int Column = 0; Column < GridHeight; ++Column)
		{
			FCell& Cell = Grid->Cells[Row][Column];
			DrawDebugBox(GetWorld(), FVector(Cell.WorldPos.X, Cell.WorldPos.Y, 10), FVector(Grid->CellSize, Grid->CellSize, 1), FColor::Blue, false, -1.0f, 0, 2.0f);
		}
	}
}

void AWFCManager::CreateTiles()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating Tiles Aleliuja 2025"));

	FTileData UnknownTile((int)ETileType::Unknown, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Tree, (int)ETileType::Path, (int)ETileType::Empty });
	UnknownTile.SetMeshString(FString("/Script/Engine.StaticMesh'/Game/Nimikko_WesternTown/Assets/Props/SM_Crate_03.SM_Crate_03'"));
	Tiles.Add(UnknownTile);
	UE_LOG(LogTemp, Warning, TEXT("Tile number One: TileMeshUnknown" ));
	
	FTileData GrassTile((int)ETileType::Grass, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Tree, (int)ETileType::Path, (int)ETileType::Empty });
	GrassTile.SetMeshString(FString("/Script/Engine.StaticMesh'/Game/Fantastic_Village_Pack/meshes/environment/SM_ENV_PLANT_grass_village.SM_ENV_PLANT_grass_village'"));
	Tiles.Add(GrassTile);
	UE_LOG(LogTemp, Warning, TEXT("Tile number Two: %i"), (int)ETileType::Grass);

	FTileData TreeTile((int)ETileType::Tree, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Empty });
	TreeTile.SetMeshString(FString( "/Script/Engine.StaticMesh'/Game/Fantastic_Village_Pack/meshes/environment/SM_ENV_TREE_village_LOD0.SM_ENV_TREE_village_LOD0'" ));
	Tiles.Add(TreeTile);

	FTileData BushTile((int)ETileType::Bush, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Tree, (int)ETileType::Path });
	BushTile.SetMeshString(FString("/Script/Engine.StaticMesh'/Game/Fantastic_Village_Pack/meshes/environment/SM_ENV_PLANT_leaf_village.SM_ENV_PLANT_leaf_village'"));
	Tiles.Add(BushTile);

	FTileData PathTile((int)ETileType::Path, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Path });
	PathTile.SetMeshString(FString("/Script/Engine.StaticMesh'/Game/Fantastic_Village_Pack/meshes/props/natural/SM_PROP_hay_01.SM_PROP_hay_01'"));
	Tiles.Add(PathTile);

	FTileData EmptyTile((int)ETileType::Empty, TArray<uint8>{ (int)ETileType::Grass, (int)ETileType::Tree, (int)ETileType::Path, (int)ETileType::Empty });
	EmptyTile.SetMeshString(FString("/Script/Engine.StaticMesh'/Game/Nimikko_WesternTown/Assets/Structure/SM_Pillar_02.SM_Pillar_02'"));
	Tiles.Add(EmptyTile);
}

void AWFCManager::InitializeGrid()
{
	Grid = GetWorld()->SpawnActor<AWFCGrid>(AWFCGrid::StaticClass());

	int Rows = GridWidth;
	int Columns = GridHeight;

	Grid->Cells.SetNum(Rows);
	for (int Row = 0; Row < Rows; ++Row)
	{
		Grid->Cells[ Row ].SetNum(Columns);  // Create columns for each row
	}

	for (int Row = 0; Row < Rows; ++Row)
	{
		for (int Column = 0; Column < Columns; ++Column)
		{
			FCell& Cell = Grid->Cells[ Row ][ Column ];
			FVector ParentPos = GetActorLocation();
			Cell.WorldPos = FVector2D(ParentPos.X, ParentPos.Y) + FVector2D(Row * Grid->CellSize, Column * Grid->CellSize);

			// Add all tile IDs to the cell's possible tiles
			for (const FTileData& Tile : Tiles)
			{
				Cell.PossibleTiles.Add(Tile.TileID);
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
	for (int Row = 0; Row < GridWidth; Row++)
	{
		for (int Column = 0; Column < GridHeight; Column++)
		{
			FCell& Cell = Grid->Cells[Row][Column];

			FVector Location = FVector(Cell.WorldPos.X - (Grid->CellSize / 2), Cell.WorldPos.Y - (Grid->CellSize / 2), 1.0f);
			
			// spawn the first tile in the list (in the end it will be the chosen one)
			uint8 ChosenTile = Cell.PossibleTiles[0];
			ATile* Tile = SpawnTile(ChosenTile);
			
			if (Tile)
			{
				Tile->SetActorLocation(Location);

				// Assign a default static mesh
				//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.Blueprint'/Game/WFC/TileMeshUnknown.TileMeshUnknown'"));
				//UStaticMesh* MeshAsset = LoadObject<UStaticMesh> (nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Fantastic_Village_Pack/meshes/environment/SM_ENV_TREE_village_LOD0.SM_ENV_TREE_village_LOD0'"));

				UStaticMesh* MeshAsset = LoadObject<UStaticMesh> (nullptr, GetMeshString(ChosenTile));

				if (MeshAsset)
				{
					Tile->FindComponentByClass<UStaticMeshComponent>()->SetStaticMesh(MeshAsset);
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawning Grid"));
}
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

void AWFCManager::OnProceduralGeneration()
{
	UE_LOG(LogTemp, Warning, TEXT("Procedural Generation"));

	InitializeGrid();
	SpawnGrid();
}

ATile* AWFCManager::SpawnTile(uint8 TileID)
{
	FActorSpawnParameters SpawnParams;

	ATile* NewTile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass());

	return NewTile;
}

const TCHAR* AWFCManager::GetMeshString(uint8 TileID)
{
	for (FTileData& Tile : Tiles)
	{
		if (Tile.TileID == TileID)
		{
			return *Tile.MeshString;
		}
	}
	return nullptr;
}