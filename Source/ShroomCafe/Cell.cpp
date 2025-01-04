// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"
#include "WFCManager.h"


//// Sets default values
//ACell::ACell()
//{
// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//}
//
//// Called when the game starts or when spawned
//void ACell::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void ACell::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

// return the position where mesh will be spawned
FVector FCell::GetWorldPos() const 
{ 
	FVector ParentPos = Grid->GetActorLocation();

	// 2D world position for the cell
	FVector2D WorldPos = FVector2D(ParentPos.X, ParentPos.Y) + FVector2D((GridPos.X * Size) + (Size / 2), (GridPos.Y * Size) + (Size / 2));
	// add height offset for 3D position
	return FVector(WorldPos, ParentPos.Z + GroundOffset);
}

void FCell::SetWeight(uint8 TileID, float Weight)
{
	if (WFWeights.Num() == 0)
		WFWeights.Init(-10.0f, (int)ETileType::Max_Tiles);

	WFWeights[ TileID ] = Weight;
}
