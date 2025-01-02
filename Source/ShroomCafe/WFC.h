#pragma once 

#include "WFCManager.h"
#include "Math/Vector.h"


namespace WFCAlgorithm
{
	void PropagateConstraints(FCell* Cell);
	void CollapseCell(FCell* Cell);
	void GenerateWaveFunction(TArray< FTileData >& Tiles);

	int SweepFirstLayer(FCell* Cell, uint8 TileID);
	int SweepSecondLayer(FCell* Cell, uint8 TileID);
}

namespace WFCWeightRules
{
	// IN current Path cell, OUT next Path cell (to be collapsed)
	FCell* PathRule(FCell* CurrentCell, TArray< FCell* >& NextCells);

	// Other rules
	float TreeRule(FCell* Cell);
	float BushRule(FCell* Cell);
	float GrassRule(FCell* Cell);
}

namespace WFCWeightUtils
{
	void AddRandomWeight(float& weight);
	int GetHighestWeightID(const TArray< float >& weights);
}
