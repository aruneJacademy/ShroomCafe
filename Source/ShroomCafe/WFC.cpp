#include "WFC.h"
#include "WFCManager.h"

// later to be loaded into json
namespace WFCData
{
	// Weight will be adjusted for each wave function element, based on these weights
	TArray<std::unordered_map< ETileType, float >> FirstLayerWeights
	{	
		{	// Unknown Tile = 0, not needed really
			{ ETileType::Path, 0.0f },
			{ ETileType::Tree, 0.0f },
			{ ETileType::Grass, 0.0f },
			{ ETileType::Bush, 0.0f },
		},
		{	// Path Tile = 1, also not needed...
			{ ETileType::Path, 0.0f },
			{ ETileType::Tree, 0.0f },
			{ ETileType::Grass, 0.0f },
			{ ETileType::Bush, 0.0f },
		},
		{	// Tree Tile = 2
			{ ETileType::Path, -1.0f },
			{ ETileType::Tree, -1.0f },
			{ ETileType::Grass, 0.1f },
			{ ETileType::Bush, 0.2f },
		},
		{	// Grass Tile = 3 
			{ ETileType::Path, 1.0f },
			{ ETileType::Tree, 0.2f },
			{ ETileType::Grass, 0.1f },
			{ ETileType::Bush, 0.2f },
		},
		{	// Bush Tile = 4
			{ ETileType::Path, -1.0f },
			{ ETileType::Tree, 0.2f },
			{ ETileType::Grass, 0.1f },
			{ ETileType::Bush, -0.1f },
		}
	};

	TArray<std::unordered_map< ETileType, float >> SecondLayerWeights
	{
		{	// Unknown Tile = 0, not needed really
			{ ETileType::Path, 0.0f },
			{ ETileType::Tree, 0.0f },
			{ ETileType::Grass, 0.0f },
			{ ETileType::Bush, 0.0f },
		},
		{	// Path Tile = 1, also not needed...
			{ ETileType::Path, 0.0f },
			{ ETileType::Tree, 0.0f },
			{ ETileType::Grass, 0.0f },
			{ ETileType::Bush, 0.0f },
		},
		{	// Tree Tile = 2 
			{ ETileType::Path, -0.2f },
			{ ETileType::Tree, -0.3f },
			{ ETileType::Grass, 0.2f },
			{ ETileType::Bush, 0.1f },
		},
		{	// Grass Tile = 3
			{ ETileType::Path, 1.0f },
			{ ETileType::Tree, 0.2f },
			{ ETileType::Grass, 0.2f },
			{ ETileType::Bush, 0.2f },
		},
		{	// Bush Tile = 4
			{ ETileType::Path, 0.3f },
			{ ETileType::Tree, 0.1f },
			{ ETileType::Grass, 0.1f },
			{ ETileType::Bush, -0.1f },
		}
	};
}

namespace WFCAlgorithm
{
	
	// Reduce entropy of the surrounding cells
	void PropagateConstraints(FCell* Cell)
	{
		// tile IDs correspond to positions in array, therefore no removing them happens
		// instead weight is set to -10.0f and later is ignored

		uint8 TileID = Cell->WaveFunction[ 0 ];
		FTileData* CollapsedTile = &Cell->Grid->GetTiles()[ TileID ];

		SweepLayer(Cell->Grid->GetGrid(), Cell, TileID, 1,
			[ &Cell, CollapsedTile ](FCell* ACell, uint8 TileID)
			{
				for (int i = 0; i < ACell->WaveFunction.Num(); i++)
				{
					if (!CollapsedTile->IsAllowedNeighbor(ACell->WaveFunction[ i ]))
					{
						ACell->WFWeights[ i ] = -10.0f;
					}
				}
			}
		);
	}

	void CollapseCell(FCell* Cell)
	{
		// Check all weights and find highest (ignore -1)
		// Collapse wave function to single tile of that weight
		// Set WaveFunction to single tile

		int TileID = WFCWeightUtils::GetHighestWeightID(Cell->WFWeights);
		if (TileID == -1) return;

		Cell->WaveFunction.Empty();
		Cell->WaveFunction.Add(TileID);
		Cell->bIsCollapsed = true;
	}

	void GenerateWaveFunction(TArray< FTileData >& Tiles)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating Tiles Aleliuja 2025"));

		FTileData UnknownTile((int)ETileType::Unknown, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Tree, (int)ETileType::Path });
		UnknownTile.SetMeshString(FString("/Script/Engine.StaticMesh'/Game/Nimikko_WesternTown/Assets/Props/SM_Crate_03.SM_Crate_03'"));
		UnknownTile.TileID;
		Tiles.Add(UnknownTile);
		UE_LOG(LogTemp, Warning, TEXT("Tile number One: TileMeshUnknown"));

		FTileData PathTile((int)ETileType::Path, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Path });
		PathTile.SetMeshString(FString("/Script/Engine.StaticMesh'/Game/Fantastic_Village_Pack/meshes/props/natural/SM_PROP_hay_01.SM_PROP_hay_01'"));
		Tiles.Add(PathTile);

		FTileData TreeTile((int)ETileType::Tree, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass });
		TreeTile.SetMeshString(FString("/Script/Engine.StaticMesh'/Game/Fantastic_Village_Pack/meshes/environment/SM_ENV_TREE_village_LOD0.SM_ENV_TREE_village_LOD0'"));
		Tiles.Add(TreeTile);

		FTileData GrassTile((int)ETileType::Grass, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Tree, (int)ETileType::Path });
		GrassTile.SetMeshString(FString("/Script/Engine.StaticMesh'/Game/Fantastic_Village_Pack/meshes/environment/SM_ENV_PLANT_grass_village.SM_ENV_PLANT_grass_village'"));
		Tiles.Add(GrassTile);
		UE_LOG(LogTemp, Warning, TEXT("Tile number Two: %i"), (int)ETileType::Grass);

		FTileData BushTile((int)ETileType::Bush, TArray<uint8>{ (int)ETileType::Bush, (int)ETileType::Grass, (int)ETileType::Tree, (int)ETileType::Path });
		BushTile.SetMeshString(FString("/Script/Engine.StaticMesh'/Game/Fantastic_Village_Pack/meshes/environment/SM_ENV_PLANT_leaf_village.SM_ENV_PLANT_leaf_village'"));
		Tiles.Add(BushTile);

		UE_LOG(LogTemp, Warning, TEXT("Wave Function Generated!"));
	}

	int SweepLayer(AWFCGrid* Grid, FCell* Cell, uint8 TileID, int32 Layer, std::function< void(FCell* Cell, uint8 TileID) >Func)
	{
		// -1 means no cells are collapsed
		// 0 means there are no such tiles in the second layer

		int ret = -1;

		// before moving to next cell, check if it exists
		// jump to cell in position X - Layer, Y - Layer
		// move right x Layer times
		// move down x Layer times
		// move left x Layer times
		// move up x Layer times
		// call std::function for each cell
		// accumulate what std::function returns
		// return accumulated value

		return -1;
	}
}

namespace WFCWeightUtils
{
	int32 gAccumulator{ 0 };
	

	void AddRandomWeight(float& weight)
	{
		float MaxWeight = 0.4f;
		float MinWeight = -0.4f;

		weight += FMath::FRandRange(MinWeight, MaxWeight);
	}

	int GetHighestWeightID(const TArray<float>& weights)
	{
		float highest = -1.0f;
		int ret = -1;

		for (int i = 0; i < weights.Num(); i++)
		{
			if (weights[ i ] == -10.0f) continue;
			else if (weights[ i ] > highest)
			{
				highest = weights[ i ];
				ret = i;
			}
		}
		return ret;
	}

	auto FindTiles = [&](FCell* Cell, uint8 TileID) -> int
	{
			if (Cell->IsCollapsed()) return 0;
			else if (Cell->WaveFunction.Num() == 0) return -1;
			else if (Cell->WaveFunction[ 0 ] == TileID)
			{
				// this might be a local variable in Sweep Layer function
				gAccumulator++;
				return 0;
			}
			return -1;
	};


	float AccumulateWeights(AWFCGrid* Grid, FCell* Cell, uint8 TileID, int Layer)
	{
		float ret = 0.0f;

		// i = any other possible tile
		for (int i = 1; i < Cell->WaveFunction.Num(); i++)
		{
			const std::unordered_map<ETileType, float>& WeightMap = WFCData::FirstLayerWeights[ TileID ];
			int TilesFound = WFCAlgorithm::SweepLayer(Grid, Cell, i, Layer, FindTiles);

			auto It = WeightMap.find(static_cast< ETileType >(i));
			float Add = TilesFound * It->second;
			ret += Add;		
		}
		return ret;
	}
}

namespace WFCWeightRules
{
	float StartWeight = 1.0f;
	float FairBias = 0.2f;

	// for PATH:
	// IN current Path cell, OUT next Path cell (to be collapsed)
	FCell* PathRule(FCell* CurrentCell, TArray< FCell* >& NextCells)
	{
		if (NextCells.Num() == 0) return nullptr;

		TArray <float> Weights;
		Weights.Init(1.0f, NextCells.Num());

		for (int i = 0; i < Weights.Num(); i++)
		{
			/*if (WFCWeightUtils::bAgainstOdds)
				Weights[ i ] += WFCData::PathDistanceWeights[ Weights.Num() - 1 - i ];
			else*/
				Weights[ i ] += WFCData::PathDistanceWeights[ i ];
		}

		for (float& Weight : Weights)
		{
			WFCWeightUtils::AddRandomWeight(Weight);
		}

		int winner = WFCWeightUtils::GetHighestWeightID(Weights);

		if (winner == -1) return nullptr;
		// 1. Calculate distance for each cell from the target and sort it (Nearest to Farthest) 3 cells in total
		// 2. Deduct weight to each cell based on the distance (-0.2f, -0.5f, -0.8f).
		// 3. Add a random weight to each cell
		// 4. Return the cell with the highest weight
		return NextCells[ winner ];
	}

	// for Other tiles:
	// return the weight for single wave function tile
	float TreeRule(FCell* Cell)
	{
		float ret = StartWeight;
		ret -= FairBias;
		// --fair bias 0.2f
		// Trees in 1st layer? -1.0f 
		// Trees in 2nd layer? --0.3f per tree
		// Any Path in 1st layer? -1.0f
		// Any Path in 2nd layer? -0.5f
		// Bush in 1st layer? ++0.2f per bush
		// Bush in 2nd layer? ++0.1f per bush
		// Add random weight

		float AmbientWeight = WFCWeightUtils::AccumulateWeights(Cell->Grid->GetGrid(), Cell, (uint8)ETileType::Tree, 1);
		AmbientWeight += WFCWeightUtils::AccumulateWeights(Cell->Grid->GetGrid(), Cell, (uint8)ETileType::Tree, 2);

		ret += AmbientWeight;
		return ret;
	}

	float GrassRule(FCell* Cell)
	{
		float ret = StartWeight;
		ret -= FairBias;
		// --fair bias 0.2f
		// grass in 1st layer? ++0.2f per grass
		// grass in 2nd layer? ++0.1f per grass
		// Add random weight
		float AmbientWeight = WFCWeightUtils::AccumulateWeights(Cell->Grid->GetGrid(), Cell, (uint8)ETileType::Grass, 1);
		AmbientWeight += WFCWeightUtils::AccumulateWeights(Cell->Grid->GetGrid(), Cell, (uint8)ETileType::Grass, 2);

		ret += AmbientWeight;
		return ret;
	}

	float BushRule(FCell* Cell)
	{
		float ret = StartWeight;
		ret -= FairBias;
		// --fair bias 0.2f
		// Any Paths in 1st layer? -1.0f
		// Trees in 1st layer? ++0.2f per tree
		// Trees in 2nd layer? ++0.1f per tree
		// Any Bush around ? --0.1f per bush
		// Add random weight
		float AmbientWeight = WFCWeightUtils::AccumulateWeights(Cell->Grid->GetGrid(), Cell, (uint8)ETileType::Bush, 1);
		AmbientWeight += WFCWeightUtils::AccumulateWeights(Cell->Grid->GetGrid(), Cell, (uint8)ETileType::Bush, 2);

		ret += AmbientWeight;
		return ret;
	}
}

