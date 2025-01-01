// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cell.h"
#include "Tile.h"
#include "WFCGrid.generated.h"

UENUM(BlueprintType)
enum class ETileType: uint8
{
	Unknown UMETA(DisplayName = "Unknown"),
	Grass UMETA(DisplayName = "Grass"),
	Tree UMETA(DisplayName = "Tree"),
	Bush UMETA(DisplayName = "Bush"),
	Path UMETA(DisplayName = "Path"),
	Empty UMETA(DisplayName = "Empty"),
	
	Max_Tiles UMETA(DisplayName = "Max_Tiles")
};


UCLASS()
class SHROOMCAFE_API AWFCGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	AWFCGrid() {}
	// Sets default values for this actor's properties
	AWFCGrid(int rows, int columns, TArray<FTileData>& tiles);

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 CellSize = 200;

	TArray< TArray<FCell> > Cells;
	
	TArray<FTileData> PossibleTiles;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
