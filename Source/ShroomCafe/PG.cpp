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

		WFCAlgorithm::CollapseCell(Start);

		while (!bTargetReached)
		{
			GetSurroundingCells(CurrentCell, NextCells);
			// sort by distance to target
			WFCUtils::SortByDistance(End, NextCells);
			// apply path rule to get next cell
			FCell* NextCell = WFCWeightRules::PathRule(CurrentCell, NextCells);

			if (!NextCell)
			{
				UE_LOG(LogTemp, Warning, TEXT("Path Generation Failed!"));
				return;
			}

			//WFCWeightUtils::bAgainstOdds = NextCell != NextCells[ 0 ];

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
	FCell* FindLowestEntropyCell(AWFCGrid* Grid)
	{
		int LowestEntropy = 10;
		FCell* LowestEntropyCell = nullptr;

		for (TArray< FCell >& Row : Grid->GetCells())
		{
			for (FCell& Cell : Row)
			{
				if (!Cell.bIsCollapsed)
				{
					if (Cell.WaveFunction.Num() < LowestEntropy)
					{
						LowestEntropy = Cell.WaveFunction.Num();
						LowestEntropyCell = &Cell;
					}
				}
			}
		}
		return LowestEntropyCell;
	}


	void Generate(AWFCGrid* Grid)
	{
		bool bIsGridEmpty = false;

		while (!bIsGridEmpty)
		{
			FCell* Cell = FindLowestEntropyCell(Grid);

			if (!Cell)
			{
				bIsGridEmpty = true;
				break;
			}

			for (uint8 TileID : Cell->WaveFunction)
			{
				switch (TileID)
				{
				case static_cast<int>(ETileType::Tree):
					Cell->SetWeight(TileID, WFCWeightRules::TreeRule(Cell));
					break;

				case static_cast<int>(ETileType::Bush):
					Cell->SetWeight(TileID, WFCWeightRules::BushRule(Cell));
					break;

				case static_cast<uint8>(ETileType::Grass):
					Cell->SetWeight(TileID, WFCWeightRules::GrassRule(Cell));
					break;

				default:
					break;
				}
			}
			WFCAlgorithm::CollapseCell(Cell);
			WFCAlgorithm::PropagateConstraints(Cell);
		}
	}
}