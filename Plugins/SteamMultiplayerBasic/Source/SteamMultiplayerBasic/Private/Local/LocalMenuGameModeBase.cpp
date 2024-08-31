// Fill out your copyright notice in the Description page of Project Settings.


#include "Local/LocalMenuGameModeBase.h"
#include "Local/LocalMainMenu.h"

ALocalMenuGameModeBase::ALocalMenuGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ULocalMainMenu> MainMenuWidgetWBPClass(TEXT("/SteamMultiplayerBasic/UI/Local/WBP_LocalMainMenu.WBP_LocalMainMenu_C"));
	if(MainMenuWidgetWBPClass.Class)
	{
		MainMenuWidgetClass = MainMenuWidgetWBPClass.Class;
	}
}

void ALocalMenuGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;

		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
	}
	
	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<ULocalMainMenu>(GetWorld(), MainMenuWidgetClass);

		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
		}
	}
}
