// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

class AWFCManager;

USTRUCT(BlueprintType)
struct FCell
{
	GENERATED_BODY()

public:
	FIntPoint GridPos{ 0, 0 };
	bool bIsCollapsed{ false };

	float Size{ 0.0f };
	float GroundOffset{ 0.0f };

	TArray< uint8 > WaveFunction;
	TArray< float > WFWeights;
	AWFCManager* Grid{ nullptr };

	FCell() {}
	FVector GetWorldPos() const;
	FIntPoint GetGridPos() const { return GridPos; }
	void SetWeight(uint8 TileID, float Weight);
	bool IsCollapsed() const { return bIsCollapsed; }
	float GetWeight(uint8 TileID) const { return WFWeights[ TileID ]; }
};

//UCLASS()
//class SHROOMCAFE_API ACell : public AActor
//{
//	GENERATED_BODY()
//	
//public:	
//	// Sets default values for this actor's properties
//	ACell();
//
//	int32 CellID{ 0 };
//	bool bIsCollapsed{ false };
//
//	FVector2D WorldPos;
//	TArray<int8> WaveFunction;
//
//	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
//
//	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//
//
//};
