// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"



USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()

public:

	FVector2D WorldPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	uint8 TileID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	TArray<uint8> AllowedNeighbors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	FString MeshString;

	void SetMeshString(FString Mesh) { MeshString = Mesh; }

	FTileData() {}
	FTileData(uint8 ID, TArray<uint8> Neighbors) :
		TileID(ID), AllowedNeighbors(Neighbors) {}
};



UCLASS()
class SHROOMCAFE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();
	//ATile(FTile* Tile) : TileData(Tile) {}

	//FTileData* TileData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
};
