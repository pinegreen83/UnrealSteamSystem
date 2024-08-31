// Fill out your copyright notice in the Description page of Project Settings.


#include "Steam/SteamMainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Steam/SteamMultiplayerMenu.h"

void USteamMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	MultiplayerMenu->SetVisibility(ESlateVisibility::Hidden);
	
	if(SinglePlayerButton)
	{
		SinglePlayerButton->OnClicked.AddDynamic(this, &USteamMainMenu::OnSinglePlayerButtonClicked);
	}

	if(MultiplayerButton)
	{
		MultiplayerButton->OnClicked.AddDynamic(this, &USteamMainMenu::OnMultiPlayerButtonClicked);
	}
}

void USteamMainMenu::OnSinglePlayerButtonClicked()
{
	ToggleMenuElements(ESlateVisibility::Hidden);

	FString LevelName = "SinglePlayLevel";
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}

void USteamMainMenu::OnMultiPlayerButtonClicked()
{
	ToggleMenuElements(ESlateVisibility::Hidden);

	if(MultiplayerMenu)
	{
		MultiplayerMenu->InitializeMainMenu(this);
		MultiplayerMenu->SetVisibility(ESlateVisibility::Visible);
		MultiplayerMenu->InitializeWidget();
	}
}

void USteamMainMenu::ToggleMenuElements(ESlateVisibility NewVisibility)
{
	// 메인 메뉴에 있는 위젯들 숨기기.
	GameTitle->SetVisibility(NewVisibility);
	SinglePlayerButton->SetVisibility(NewVisibility);
	MultiplayerButton->SetVisibility(NewVisibility);
}
