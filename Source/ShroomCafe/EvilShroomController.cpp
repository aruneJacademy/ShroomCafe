// Fill out your copyright notice in the Description page of Project Settings.



#include "EvilShroomController.h"





AEvilShroomController::AEvilShroomController()
{
	// Initialize the delegate
	//OnChangeAnimation = FOnChangeAnimation();
}


void AEvilShroomController::NotifyChangeAnimation(FName NewAnimState)
{
	OnChangeAnimation.Broadcast(NewAnimState); // Broadcast the delegate
}