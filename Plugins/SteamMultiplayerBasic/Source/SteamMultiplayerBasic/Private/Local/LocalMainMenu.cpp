// Fill out your copyright notice in the Description page of Project Settings.


#include "Local/LocalMainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Local/LocalMultiplayerMenu.h"

void ULocalMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	MultiplayerMenu->SetVisibility(ESlateVisibility::Hidden);
	
	if(SinglePlayerButton)
	{
		SinglePlayerButton->OnClicked.AddDynamic(this, &ULocalMainMenu::OnSinglePlayerButtonClicked);
	}

	if(MultiplayerButton)
	{
		MultiplayerButton->OnClicked.AddDynamic(this, &ULocalMainMenu::OnMultiPlayerButtonClicked);
	}
}

void ULocalMainMenu::OnSinglePlayerButtonClicked()
{
	ToggleMenuElements(ESlateVisibility::Hidden);

	FString LevelName = "SinglePlayLevel";
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}

void ULocalMainMenu::OnMultiPlayerButtonClicked()
{
	ToggleMenuElements(ESlateVisibility::Hidden);

	if(MultiplayerMenu)
	{
		MultiplayerMenu->InitializeMainMenu(this);
		MultiplayerMenu->SetVisibility(ESlateVisibility::Visible);
		MultiplayerMenu->InitializeWidget();
	}
}

void ULocalMainMenu::ToggleMenuElements(ESlateVisibility NewVisibility)
{
	// 메인 메뉴에 있는 위젯들 숨기기.
	GameTitle->SetVisibility(NewVisibility);
	SinglePlayerButton->SetVisibility(NewVisibility);
	MultiplayerButton->SetVisibility(NewVisibility);
}
