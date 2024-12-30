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

	// pull tiles from json later 
	void InitializeGrid(int rows, int columns, TArray<ATile>& tiles);
	void GenerateGrid();
	void SpawnGrid();

	TArray<FTileData> Tiles;
	void CreateTiles();
	UStaticMeshComponent* GetTileMesh(FString TargetName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
