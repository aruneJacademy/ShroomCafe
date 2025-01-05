// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WFCGrid.h"
#include "WFCManager.generated.h"



UCLASS()
class SHROOMCAFE_API AWFCManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWFCManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DrawDebugGrid();
	const TCHAR* GetMeshString(uint8 TileID);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// pull tiles from json later 
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void InitializeGrid();

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void GenerateGrid();

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SpawnGrid();

	// will be promoted to delegate later
	void OnProceduralGeneration();

	int32 GetGridWidth() const { return GridWidth; }
	int32 GetGridHeight() const { return GridHeight; }
	TArray<FTileData >& GetTiles() { return Tiles; }
	AWFCGrid* GetGrid() { return Grid; }
	static const float EntropyThreshold;
	
protected:
	TArray< FTileData > Tiles;
	AWFCGrid* Grid{ nullptr };
	bool bGenerated{ false };

	// height and width is swapped unintentionally, but at this point and time it's too painful to fix
	// therefore height is width and width is height
	int32 GridWidth = 50;
	int32 GridHeight = 100;
	float VerticalOffset = 0.0f;
};
