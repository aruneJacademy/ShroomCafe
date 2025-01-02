#pragma once

#include "CoreMinimal.h"

class AWFCGrid;
struct FCell;



namespace ProceduralPath
{
	void Generate(AWFCGrid* Grid, FCell* Start, FCell* End);
	void GetSurroundingCells(FCell* Current, TArray< FCell* >& Next);
}

namespace ProceduralWorld
{
	void Generate(AWFCGrid* Grid);
	FCell* FindLowestEntropyCell(AWFCGrid* Grid);
}