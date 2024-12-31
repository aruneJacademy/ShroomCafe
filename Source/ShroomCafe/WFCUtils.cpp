// Fill out your copyright notice in the Description page of Project Settings.


#include "WFCUtils.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/StaticMeshActor.h>

WFCUtils::WFCUtils()
{

}

WFCUtils::~WFCUtils()
{

}

UStaticMeshComponent* WFCUtils::GetTileMesh(FString TargetName, UWorld* World)
{
	//UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	// Array to store all actors of the desired class
	TArray<AActor*> FoundActors;

	// Find all actors of the desired class (e.g., AStaticMeshActor)
	UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (Actor && Actor->GetName() == TargetName)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Actor: %s"), *Actor->GetName());

			// Cast to AStaticMeshActor to access the StaticMeshComponent
			AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(Actor);
			if (MeshActor)
			{
				UStaticMeshComponent* MeshComponent = MeshActor->GetStaticMeshComponent();
				if (MeshComponent)
				{
					UE_LOG(LogTemp, Warning, TEXT("Static Mesh Name: %s"), *MeshComponent->GetStaticMesh()->GetName());
					return MeshComponent;
				}
			}
			return nullptr; // Exit the function after finding the actor
		}
	}
	return nullptr;
}