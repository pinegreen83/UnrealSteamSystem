// Fill out your copyright notice in the Description page of Project Settings.


#include "Steam/SteamMenuGameModeBase.h"
#include "Steam/SteamMainMenu.h"

ASteamMenuGameModeBase::ASteamMenuGameModeBase()
{
	static ConstructorHelpers::FClassFinder<USteamMainMenu> MainMenuWidgetWBPClass(TEXT("/SteamMultiplayerBasic/UI/Steam/WBP_SteamMainMenu.WBP_SteamMainMenu_C"));
	if(MainMenuWidgetWBPClass.Class)
	{
		MainMenuWidgetClass = MainMenuWidgetWBPClass.Class;
	}
}

void ASteamMenuGameModeBase::BeginPlay()
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
		MainMenuWidget = CreateWidget<USteamMainMenu>(GetWorld(), MainMenuWidgetClass);

		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
		}
	}
}
