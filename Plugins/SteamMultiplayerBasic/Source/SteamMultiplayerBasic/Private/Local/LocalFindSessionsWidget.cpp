// Fill out your copyright notice in the Description page of Project Settings.


#include "Local/LocalFindSessionsWidget.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Local/LocalMultiplayerMenu.h"
#include "Local/LocalSessionInfoWidget.h"

void ULocalFindSessionsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(ReturnToMainButton)
	{
		ReturnToMainButton->OnClicked.AddDynamic(this, &ULocalFindSessionsWidget::OnReturnToMainButtonClicked);
	}
}

void ULocalFindSessionsWidget::OnReturnToMainButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	
	if(MultiplayerMenu)
	{
		MultiplayerMenu->ToggleMenuElements(ESlateVisibility::Visible);
	}
}

void ULocalFindSessionsWidget::PopulateSessionList()
{
	if(SessionListScrollBox && SessionSearch.IsValid())
	{
		SessionListScrollBox->ClearChildren();
		
		for(const auto& SearchResult : SessionSearch->SearchResults)
		{
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.f,
					FColor::Emerald,
					FString(TEXT("Show Find Session")));
			}
			
			ULocalSessionInfoWidget* SessionWidget = CreateWidget<ULocalSessionInfoWidget>(GetWorld(), LocalSessionInfoWidgetClass);
			if(SessionWidget)
			{
				SessionWidget->SetSessionInfo(SearchResult);
				SessionListScrollBox->AddChild(SessionWidget);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Can't Make SessionInfoWidget"));
				if(GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,
						15.f,
						FColor::Emerald,
						FString(TEXT("Can't Make SessionInfoWidget")));
				}
			}
		}
	}
}

void ULocalFindSessionsWidget::InitializeMultiplayerMenu(TObjectPtr<class ULocalMultiplayerMenu> InMultiplayerMenu)
{
	MultiplayerMenu = InMultiplayerMenu;
}

void ULocalFindSessionsWidget::GetSessionSearchResult(TSharedPtr<class FOnlineSessionSearch> InSessionSearch)
{
	SessionSearch = InSessionSearch;
}
