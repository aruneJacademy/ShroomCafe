#include "PG.h"
#include "WFCManager.h"
#include "WFC.h"
#include "WFCUtils.h"


namespace ProceduralPath
{
	void Generate(AWFCGrid* Grid, FCell* Start, FCell* End)
	{
		FCell* CurrentCell = Start;
		FVector CurrentPos = CurrentCell->GetWorldPos();
		FVector TargetPos = End->GetWorldPos();
		bool bTargetReached = false;

		TArray< FCell* > NextCells;

		while (!bTargetReached)
		{
			GetSurroundingCells(CurrentCell, NextCells);
			// sort by distance to target
			WFCUtils::SortByDistance(End, NextCells);
			// apply path rule to get next cell
			FCell* NextCell = WFCWeightRules::PathRule(CurrentCell, NextCells);
			// set weight very large to make sure it collapses
			NextCell->SetWeight(static_cast< int >(ETileType::Path), 100.0f);
			WFCAlgorithm::CollapseCell(NextCell);

			CurrentCell = NextCell;
			NextCells.Empty();

			if (CurrentCell == End)
			{
				bTargetReached = true;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Path Generated!"));
	}


	void GetSurroundingCells(FCell* Current, TArray< FCell* >& Next)
	{
		FIntPoint CurrentPos = Current->GetGridPos();

		if (CurrentPos.X + 1 < Current->Grid->GetGridWidth())
		{
			FIntPoint NextPos = { CurrentPos.X + 1, CurrentPos.Y };
			FCell* NextCell = &Current->Grid->GetGrid()->GetCellAtPosition(NextPos);

			if (!NextCell->bIsCollapsed) Next.Add(NextCell);
		}
		if (CurrentPos.X - 1 >= 0)
		{
			FIntPoint NextPos = { CurrentPos.X - 1, CurrentPos.Y };
			FCell* NextCell = &Current->Grid->GetGrid()->GetCellAtPosition(NextPos);

			if (!NextCell->bIsCollapsed) Next.Add(NextCell);
		}
		if (CurrentPos.Y + 1 < Current->Grid->GetGridHeight())
		{
			FIntPoint NextPos = { CurrentPos.X, CurrentPos.Y + 1 };
			FCell* NextCell = &Current->Grid->GetGrid()->GetCellAtPosition(NextPos);

			if (!NextCell->bIsCollapsed) Next.Add(NextCell);
		}
		if (CurrentPos.Y - 1 >= 0)
		{
			FIntPoint NextPos = { CurrentPos.X, CurrentPos.Y - 1 };
			FCell* NextCell = &Current->Grid->GetGrid()->GetCellAtPosition(NextPos);

			if (!NextCell->bIsCollapsed) Next.Add(NextCell);
		}
	}
}

namespace ProceduralWorld
{
	void Generate(AWFCGrid* Grid)
	{

	}

	FCell* FindLowestEntropyCell(AWFCGrid* Grid)
	{
		return nullptr;
	}
}