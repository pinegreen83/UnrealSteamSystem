// Fill out your copyright notice in the Description page of Project Settings.


#include "Steam/SteamFindSessionsWidget.h"

#include "OnlineSessionSettings.h"
#include "SteamMultiplayerSubsystem.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Steam/SteamMultiplayerMenu.h"
#include "Steam/SteamSessionInfoWidget.h"

void USteamFindSessionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(ReturnToMainButton)
	{
		ReturnToMainButton->OnClicked.AddDynamic(this, &USteamFindSessionsWidget::OnReturnToMainButtonClicked);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		SteamMultiplayerSubsystem = GameInstance->GetSubsystem<USteamMultiplayerSubsystem>();
	}

	if(SteamMultiplayerSubsystem)
	{
		SteamMultiplayerSubsystem->SteamMultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessionsComplete);
	}
}

void USteamFindSessionsWidget::OnReturnToMainButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	
	if(MultiplayerMenu)
	{
		MultiplayerMenu->ToggleMenuElements(ESlateVisibility::Visible);
	}
}

void USteamFindSessionsWidget::StartFindSessions()
{
	SteamMultiplayerSubsystem->FindSessions(1000);
}

void USteamFindSessionsWidget::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults,
	bool bWasSuccessful)
{
	if(SteamMultiplayerSubsystem == nullptr)
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Found %d sessions."), SessionResults.Num());
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Cyan,
			FString::Printf(TEXT("Found %d sessions."), SessionResults.Num())
			);
	}
	
	if(bWasSuccessful && SessionResults.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Found %d sessions."), SessionResults.Num());
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString::Printf(TEXT("Found %d sessions."), SessionResults.Num())
				);
		}
		PopulateSessionList(SessionResults);
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

void USteamFindSessionsWidget::PopulateSessionList(const TArray<FOnlineSessionSearchResult>& SessionResults)
{
	if(SessionListScrollBox && SessionResults.Num() > 0)
	{
		SessionListScrollBox->ClearChildren();
		
		for(const auto& SearchResult : SessionResults)
		{
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.f,
					FColor::Emerald,
					FString(TEXT("Show Find Session")));
			}
			
			USteamSessionInfoWidget* SessionWidget = CreateWidget<USteamSessionInfoWidget>(GetWorld(), SteamSessionInfoWidgetClass);
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

void USteamFindSessionsWidget::InitializeMultiplayerMenu(TObjectPtr<class USteamMultiplayerMenu> InMultiplayerMenu)
{
	MultiplayerMenu = InMultiplayerMenu;
}

