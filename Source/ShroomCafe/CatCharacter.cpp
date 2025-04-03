// Fill out your copyright notice in the Description page of Project Settings.

#include "CatCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
ACatCharacter::ACatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}

// Called when the game starts or when spawned
void ACatCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACatCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACatCharacter, CurrentHealth);
}

void ACatCharacter::OnHealthUpdate()
{
	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}
	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}
	//Functions that occur on all machines.
	/*
	Any special functionality that should occur as a result of damage or death should be placed here.
	*/

}

float ACatCharacter::TakeDamage(float damageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.f;
}

void ACatCharacter::OnRep_CurrentHealth()
{
	/*Variables replicate any time their value changes rather than constantly replicating, 
	and RepNotifies run any time the client successfully receives a replicated value for a variable.
	Therefore, any time you change the player's CurrentHealth on the server, 
	you would expect OnRep_CurrentHealth to run on each connected client. 
	This makes OnRep_CurrentHealth the ideal place to call OnHealthUpdate on clients' machines.*/
	OnHealthUpdate();
}

void ACatCharacter::SetCurrentHealth(float healthValue)
{
	// restrict this function to execute only if it is called on the hosted game server.
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		// call OnHealthUpdate to ensure, that the server and clients both have parallel calls to this function. This is necessary, because the server will not receive the RepNotify.
		OnHealthUpdate();
	}
}

