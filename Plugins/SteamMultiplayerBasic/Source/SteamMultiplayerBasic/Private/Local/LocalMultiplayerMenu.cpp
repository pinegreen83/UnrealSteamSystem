// Fill out your copyright notice in the Description page of Project Settings.


#include "Local/LocalMultiplayerMenu.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Local/LocalCreateSessionWidget.h"
#include "Local/LocalFindSessionsWidget.h"
#include "Local/LocalMainMenu.h"
#include "Online/OnlineSessionNames.h"

class IOnlineSubsystem;

void ULocalMultiplayerMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(ReturnToMainButton)
	{
		ReturnToMainButton->OnClicked.AddDynamic(this, &ULocalMultiplayerMenu::OnReturnToMainButtonClicked);
	}

	if(OpenCreateSessionPanelButton)
	{
		OpenCreateSessionPanelButton->OnClicked.AddDynamic(this, &ULocalMultiplayerMenu::OnOpenCreateSessionPanelButtonClicked);
	}

	if(OpenFindSessionsPanelButton)
	{
		OpenFindSessionsPanelButton->OnClicked.AddDynamic(this, &ULocalMultiplayerMenu::OnOpenFindSessionsPanelButtonClicked);
	}
}

/*
 * 버튼에 대한 동작 함수들
 */
void ULocalMultiplayerMenu::OnReturnToMainButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);

	if (MainMenuWidget)
    {
        MainMenuWidget->ToggleMenuElements(ESlateVisibility::Visible);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MainMenuWidget is null in OnReturnToMainButtonClicked"));
    }
}

void ULocalMultiplayerMenu::OnOpenCreateSessionPanelButtonClicked()
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

void ULocalMultiplayerMenu::OnOpenFindSessionsPanelButtonClicked()
{
	ToggleMenuElements(ESlateVisibility::Hidden);
	if(FindSessionsWidget)
	{
		FindSessions();
		CreateSessionWidget->SetVisibility(ESlateVisibility::Hidden);
		FindSessionsWidget->InitializeMultiplayerMenu(this);
		FindSessionsWidget->SetVisibility(ESlateVisibility::Visible);;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FindSessionsWidget is null in OnOpenFindSessionsPanelButtonClicked"));
	}
}

void ULocalMultiplayerMenu::FindSessions()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
		if(Sessions.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = true;
			SessionSearch->MaxSearchResults = 100;
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);


			Sessions->OnFindSessionsCompleteDelegates.AddUObject(this, &ULocalMultiplayerMenu::OnFindSessionsComplete);
			Sessions->FindSessions(0, SessionSearch.ToSharedRef());
		}
	}
}

void ULocalMultiplayerMenu::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Log, TEXT("Found %d sessions."), SessionSearch->SearchResults.Num());
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Cyan,
			FString::Printf(TEXT("Found %d sessions."), SessionSearch->SearchResults.Num())
			);
	}
	
	if(bWasSuccessful && SessionSearch->SearchResults.Num() > 0)
	{
		if(FindSessionsWidget)
		{
			FindSessionsWidget->GetSessionSearchResult(SessionSearch);
			FindSessionsWidget->PopulateSessionList();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Sessions Found."));
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString(TEXT("No Sessions Found."))
				);
		}
	}
}

// 위젯을 초기화 하는 함수.
void ULocalMultiplayerMenu::InitializeWidget()
{
	ToggleMenuElements(ESlateVisibility::Visible);
	CreateSessionWidget->SetVisibility(ESlateVisibility::Hidden);
	FindSessionsWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ULocalMultiplayerMenu::ToggleMenuElements(ESlateVisibility NewVisibility)
{
	ReturnToMainButton->SetVisibility(NewVisibility);
	OpenCreateSessionPanelButton->SetVisibility(NewVisibility);
	OpenFindSessionsPanelButton->SetVisibility(NewVisibility);
}

// 멀티플레이 함수에서 메인 메뉴로 다시 돌아가기 위한 포인터를 받아오는 함수
void ULocalMultiplayerMenu::InitializeMainMenu(TObjectPtr<class ULocalMainMenu> InMainMenu)
{
	MainMenuWidget = InMainMenu;
}
