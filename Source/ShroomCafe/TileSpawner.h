// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileSpawner.generated.h"

class AWFCManager;
struct FCell;

UCLASS()
class SHROOMCAFE_API ATileSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnPath(AWFCManager* Manager, FCell* Cell, uint8 TileID);
	void SpawnTree(AWFCManager* Manager, FCell* Cell, uint8 TileID);
	void SpawnBush(AWFCManager* Manager, FCell* Cell, uint8 TileID);
	void SpawnGrass(AWFCManager* Manager, FCell* Cell, uint8 TileID);
};
