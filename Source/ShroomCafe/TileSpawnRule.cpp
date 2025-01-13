// Fill out your copyright notice in the Description page of Project Settings.


#include "TileSpawnRule.h"
#include "WFCManager.h"
#include "Components/CapsuleComponent.h"



TileSpawnRule::TileSpawnRule()
{

}

TileSpawnRule::~TileSpawnRule()
{

}

int TileSpawnRule::GetObjectCount(int TileID)
{
	switch (TileID)
	{
	case static_cast< int >(ETileType::Path): return 1;
	case static_cast< int >(ETileType::Tree): return 1;
	case static_cast< int >(ETileType::Grass): return 1/*FMath::RandRange(2, 3)*/;
	case static_cast< int >(ETileType::Bush): return 1/*FMath::RandRange(1, 2)*/;
	default: return 0;
	}
}

float TileSpawnRule::GetScale(int TileID)
{
	switch (TileID)
	{
	case static_cast< int >(ETileType::Path): return 1.0f;
	case static_cast< int >(ETileType::Tree): return FMath::RandRange(0.8f, 1.5f);
	case static_cast< int >(ETileType::Grass): return FMath::RandRange(0.7f, 1.3f);
	case static_cast< int >(ETileType::Bush): return FMath::RandRange(0.6f, 1.2f);
	default: return 1.0f;
	}
}


void TileSpawnRule::SetCapsule(ATile* Tile, int TileID)
{
	if (TileID != static_cast< int >(ETileType::Tree)) return;
	
	UCapsuleComponent* CapsuleComp = NewObject<UCapsuleComponent>(Tile, UCapsuleComponent::StaticClass());
	CapsuleComp->AttachToComponent(Tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	float height;
	float radius;

	float scale = Tile->GetActorScale().X;

	if (scale < 1.0f)
	{
		height = 100.0f;
		radius = 50.0f;
	}
	else if (scale < 1.3f)
	{
		height = 150.0f;
		radius = 60.0f;
	}
	else
	{
		height = 200.0f;
		radius = 70.0f;
	}

	CapsuleComp->SetCapsuleHalfHeight(height);
	CapsuleComp->SetCapsuleRadius(radius);

	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);  // Treats the capsule as a dynamic object
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Block);  // Block all interactions

	CapsuleComp->RegisterComponent();  // Register the component to make it functional
}

void TileSpawnRule::SetMesh(ATile* Tile, FTileData* Data, int TileID)
{
	int ObjectCount = GetObjectCount(TileID);

	for (int i = 0; i < ObjectCount; i++)
	{
		// Setup Mesh Component
		UStaticMeshComponent* NewMeshComp = NewObject<UStaticMeshComponent>(Tile);
		NewMeshComp->AttachToComponent(Tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		// Setup Mesh Asset
		UStaticMesh* MeshAsset = LoadObject< UStaticMesh >(nullptr, AWFCManager::GetMeshString(TileID));
		NewMeshComp->SetStaticMesh(MeshAsset);
		NewMeshComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		NewMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		// Setup scale and collision params
		float Scale = GetScale(TileID);
		NewMeshComp->SetWorldScale3D(FVector(Scale));
		NewMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		NewMeshComp->RegisterComponent();

		Tile->Meshes.Add(NewMeshComp);
	}
}