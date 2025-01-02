#include "WFC.h"
#include "WFCManager.h"


namespace WFCAlgorithm
{
	// Reduce entropy of the surrounding cells
	void PropagateConstraints(FCell* Cell)
	{
		// tile IDs correspond to positions in array, therefore no removing them happens
		// instead weight is set to -1.0f and later is ignored

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

	int SweepFirstLayer(FCell* Cell, uint8 TileID)
	{
		// 8 surrounding cells
		// -1 means no cells are collapsed
		// 0 means there a no such tiles in the first layer
		return -1;
	}

	int SweepSecondLayer(FCell* Cell, uint8 TileID)
	{
		// further 16 surrounding cells
		// -1 means no cells are collapsed
		// 0 means there a no such tiles in the second layer
		return -1;
	}
}

namespace WFCWeightUtils
{
	void AddRandomWeight(float& weight)
	{
		float MaxWeight = 0.3f;
		float MinWeight = -0.3f;

		weight += FMath::FRandRange(MinWeight, MaxWeight);
	}

	int GetHighestWeightID(const TArray<float>& weights)
	{
		float highest = 0.0f;
		int ret = -1;

		for (int i = 0; i < weights.Num(); i++)
		{
			if (weights[ i ] == -1.0f) continue;
			else if (weights[ i ] > highest)
			{
				highest = weights[ i ];
				ret = i;
			}
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
		// 1. Calculate distance for each cell from the target and sort it (Nearest to Farthest) 3 cells in total
		// 2. Deduct weight to each cell based on the distance (-0.2f, -0.5f, -0.8f).
		// 3. Add a random weight to each cell
		// 4. Return the cell with the highest weight
		return NextCells[ 0 ];
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
		return ret;
	}

}

