// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
