// Fill out your copyright notice in the Description page of Project Settings.


#include "WFCManager.h"
#include "WFCUtils.h"
#include "DrawDebugHelpers.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "WFC.h"
#include "PG.h"

const float AWFCManager::EntropyThreshold{ -10.0f };

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
	Grid->SetGridHeight(Columns);
	Grid->SetGridWidth(Rows);

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
			Cell.GroundOffset = VerticalOffset;
			Cell.GridPos = { Row, Column };
			Cell.Size = Grid->GetCellSize();

			FVector ParentPos = GetActorLocation();

			// Copy wave function into the cell
			for (const FTileData& Tile : Tiles)
			{
				Cell.WaveFunction.Add(Tile.TileID);
			}

			Cell.WFWeights.Init(EntropyThreshold, Cell.WaveFunction.Num());
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Grid Initialized!"));
}

void AWFCManager::GenerateGrid()
{
	ProceduralPath::Generate
	(
		Grid, 
		&Grid->GetCells()[ 0 ][ 0 ], // start of path
		&Grid->GetCells()[ GridWidth - 1 ][ GridHeight - 10 ] // end of path
	);

	ProceduralPath::Generate
	(
		Grid,
		&Grid->GetCells()[ 0 ][ GridHeight - 1 ], // start of path
		&Grid->GetCells()[ GridWidth - 1 ][ 0 ] // end of path
	);

	ProceduralPath::Generate
	(
		Grid,
		&Grid->GetCells()[ GridWidth - 1 ][ 0 ], // start of path
		&Grid->GetCells()[ 0 ][ GridHeight - 1 ] // end of path
	);

	ProceduralWorld::Generate(Grid);
}

void AWFCManager::SpawnGrid()
{
	for (int Row = 0; Row < GridWidth; Row++)
	{
		for (int Column = 0; Column < GridHeight; Column++)
		{
			FCell& Cell = Grid->GetCells()[ Row ][ Column ];

			// Tile 0 == collapsed WF
			uint8 ChosenTile = Cell.WaveFunction[ 0 ];
			ATile* Tile = WFCUtils::SpawnTile(GetWorld(), ChosenTile);
			
			if (Tile)
			{
				Tile->SetActorLocation(Cell.GetWorldPos());
				UStaticMesh* MeshAsset = LoadObject< UStaticMesh >(nullptr, GetMeshString(ChosenTile));
			
				if (MeshAsset)
				{
					UStaticMeshComponent* MeshComp = Tile->FindComponentByClass< UStaticMeshComponent >();
					MeshComp->SetStaticMesh(MeshAsset);

					FVector NewScale(1.5f, 1.5f, 1.5f);
					MeshComp->SetWorldScale3D(NewScale);
					MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}

				// Add a capsule around the tree trunk instead of mesh collisions
				if (ChosenTile == static_cast< uint8 >(ETileType::Tree))
				{
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
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Grid Spawned!"));
}

void AWFCManager::OnProceduralGeneration()
{
	UE_LOG(LogTemp, Warning, TEXT("Procedural Generation"));

	// Create grid and cells
	InitializeGrid();

	// WFC Algorithm
	GenerateGrid();

	// Spawn collapsed tiles in the world
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

