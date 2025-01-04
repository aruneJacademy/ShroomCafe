#pragma once 

#include "WFCManager.h"
#include "Math/Vector.h"
#include <functional>


class AWFCGrid;

namespace WFCAlgorithm
{
	void PropagateConstraints(FCell* Cell);
	void CollapseCell(FCell* Cell);
	void GenerateWaveFunction(TArray< FTileData >& Tiles);

	// to be used for: 
	// constraint propagation and weight accumulation
	int SweepLayer
	(
		AWFCGrid* Grid, 
		FCell* Cell, 
		uint8 TileID, 
		int32 Layer,
		std::function< void(FCell* Cell, uint8 TileID) >Func
	);

	

	// to bind std::function to a function (before pasing it ) 
	// auto BoundFunc = std::bind(&MyClass::MyMemberFunction, &Obj, std::placeholders::_1);
	// or, use lambda instead of free function - no binding needed then
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
	bool bAgainstOdds{ false };
	void AddRandomWeight(float& weight);
	int GetHighestWeightID(const TArray< float >& weights);

	float AccumulateWeights(AWFCGrid* Grid, FCell* Cell, uint8 TileID, int Layer);
}

namespace WFCData
{
	inline TArray< float >PathDistanceWeights
	{
		-0.2f, -0.4f, -0.6f, -0.2f
	};
}


