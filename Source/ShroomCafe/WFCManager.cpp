// Fill out your copyright notice in the Description page of Project Settings.


#include "WFCManager.h"
#include "WFCUtils.h"
#include "DrawDebugHelpers.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "WFC.h"
#include "PG.h"

// Sets default values
AWFCManager::AWFCManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// construction helpers are meant to be called in the constructor only. Other places need to use Loaders and Finders
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.Blueprint'/Game/WFC/TileMeshUnknown.TileMeshUnknown'"));
	WFCAlgorithm::GenerateWaveFunction(Tiles);
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
		// will be promoted to delegate later
		OnProceduralGeneration();
		bGenerated = true;
	}

	DrawDebugGrid();
}

void AWFCManager::DrawDebugGrid()
{
	for (int Row = 0; Row < GridWidth; ++Row)
	{
		for (int Column = 0; Column < GridHeight; ++Column)
		{
			FCell& Cell = Grid->GetCells()[ Row ][ Column ];
			DrawDebugBox(GetWorld(), Cell.GetWorldPos(), FVector(Grid->GetCellSize() / 2, Grid->GetCellSize() / 2, 1), FColor::Blue, false, -1.0f, 0, 2.0f);
		}
	}
}



void AWFCManager::InitializeGrid()
{
	Grid = GetWorld()->SpawnActor<AWFCGrid>(AWFCGrid::StaticClass());

	int Rows = GridWidth;
	int Columns = GridHeight;

	// Create the grid
	Grid->GetCells().SetNum(Rows);
	for (int Row = 0; Row < Rows; ++Row)
	{
		Grid->GetCells()[ Row ].SetNum(Columns);  // Create columns for each row
	}

	// Create cells
	for (int Row = 0; Row < Rows; ++Row)
	{
		for (int Column = 0; Column < Columns; ++Column)
		{
			FCell& Cell = Grid->GetCells()[ Row ][ Column ];
			Cell.Grid = this;
			Cell.GroundOffset = 10.0f;
			Cell.GridPos = { Row, Column };
			Cell.Size = Grid->GetCellSize();

			FVector ParentPos = GetActorLocation();

			// Copy wave function into the cell
			for (const FTileData& Tile : Tiles)
			{
				Cell.WaveFunction.Add(Tile.TileID);
			}

			Cell.WFWeights.Init(-10.0f, Cell.WaveFunction.Num());
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Grid Initialized!"));
}

void AWFCManager::GenerateGrid()
{
	ProceduralPath::Generate(
		Grid, 
		&Grid->GetCells()[ 0 ][ 0 ], // start of path
		&Grid->GetCells()[ GridWidth - 1 ][ GridHeight - 10 ]); // end of path

	ProceduralWorld::Generate(Grid);
}

void AWFCManager::SpawnGrid()
{
	for (int Row = 0; Row < GridWidth; Row++)
	{
		for (int Column = 0; Column < GridHeight; Column++)
		{
			FCell& Cell = Grid->GetCells()[ Row ][ Column ];

			// spawn the first tile in the list (in the end it will be the chosen one)
			uint8 ChosenTile = Cell.WaveFunction[ 0 ];
			ATile* Tile = WFCUtils::SpawnTile(GetWorld(), ChosenTile);
			
			if (Tile)
			{
				Tile->SetActorLocation(Cell.GetWorldPos());
				UStaticMesh* MeshAsset = LoadObject<UStaticMesh> (nullptr, GetMeshString(ChosenTile));

				if (MeshAsset)
				{
					Tile->FindComponentByClass<UStaticMeshComponent>()->SetStaticMesh(MeshAsset);
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Grid Spawned!"));
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

	// Create grid and cells
	InitializeGrid();
	// WFC Algorithm
	GenerateGrid();
	// Spawn collapsed tiles
	SpawnGrid();
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

