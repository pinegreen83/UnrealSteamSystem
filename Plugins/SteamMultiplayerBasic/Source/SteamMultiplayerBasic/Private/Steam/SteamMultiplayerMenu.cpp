// Fill out your copyright notice in the Description page of Project Settings.


#include "Steam/SteamMultiplayerMenu.h"
#include "Components/Button.h"
#include "Steam/SteamCreateSessionWidget.h"
#include "Steam/SteamFindSessionsWidget.h"
#include "Steam/SteamMainMenu.h"


void USteamMultiplayerMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if(ReturnToMainButton)
	{
		ReturnToMainButton->OnClicked.AddDynamic(this, &USteamMultiplayerMenu::OnReturnToMainButtonClicked);
	}

	if(OpenCreateSessionPanelButton)
	{
		OpenCreateSessionPanelButton->OnClicked.AddDynamic(this, &USteamMultiplayerMenu::OnOpenCreateSessionPanelButtonClicked);
	}

	if(OpenFindSessionsPanelButton)
	{
		OpenFindSessionsPanelButton->OnClicked.AddDynamic(this, &USteamMultiplayerMenu::OnOpenFindSessionsPanelButtonClicked);
	}
}

/*
 * 버튼에 대한 동작 함수들
 */
void USteamMultiplayerMenu::OnReturnToMainButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);

	if (MainMenuWidget)
	{
		// MainMenuWidget->ToggleMenuElements(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainMenuWidget is null in OnReturnToMainButtonClicked"));
	}
}

void USteamMultiplayerMenu::OnOpenCreateSessionPanelButtonClicked()
{
	ToggleMenuElements(ESlateVisibility::Hidden);
	if(CreateSessionWidget)
	{
		FindSessionsWidget->SetVisibility(ESlateVisibility::Hidden);
		CreateSessionWidget->InitializeMultiplayerMenu(this);
		CreateSessionWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateSessionWidget is null in OnOpenCreateSessionPanelButtonClicked"));
	}
}

void USteamMultiplayerMenu::OnOpenFindSessionsPanelButtonClicked()
{
	ToggleMenuElements(ESlateVisibility::Hidden);
	if(FindSessionsWidget)
	{
		CreateSessionWidget->SetVisibility(ESlateVisibility::Hidden);
		FindSessionsWidget->InitializeMultiplayerMenu(this);
		FindSessionsWidget->SetVisibility(ESlateVisibility::Visible);
		FindSessionsWidget->StartFindSessions();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FindSessionsWidget is null in OnOpenFindSessionsPanelButtonClicked"));
	}
}

void USteamMultiplayerMenu::InitializeWidget()
{
	ToggleMenuElements(ESlateVisibility::Visible);
	CreateSessionWidget->SetVisibility(ESlateVisibility::Hidden);
	FindSessionsWidget->SetVisibility(ESlateVisibility::Hidden);
}

void USteamMultiplayerMenu::ToggleMenuElements(ESlateVisibility NewVisibility)
{
	ReturnToMainButton->SetVisibility(NewVisibility);
	OpenCreateSessionPanelButton->SetVisibility(NewVisibility);
	OpenFindSessionsPanelButton->SetVisibility(NewVisibility);
}

void USteamMultiplayerMenu::InitializeMainMenu(TObjectPtr<class USteamMainMenu> InMainMenu)
{
	MainMenuWidget = InMainMenu;
}