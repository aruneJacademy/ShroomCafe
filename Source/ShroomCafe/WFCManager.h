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

	TArray<FTile> Tiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 GridWidth = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 GridHeight = 5;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// pull tiles from json later 
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void InitializeGrid(int rows, int columns);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void GenerateGrid();

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SpawnGrid();

	void CreateTiles();

	void OnProceduralGeneration();

	AWFCGrid* Grid{ nullptr };
	
};
