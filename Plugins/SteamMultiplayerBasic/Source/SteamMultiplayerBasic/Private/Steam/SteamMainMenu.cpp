// Fill out your copyright notice in the Description page of Project Settings.


#include "Steam/SteamMainMenu.h"
#include "Components/Button.h"
#include "SteamMultiplayerSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void USteamMainMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		SteamMultiplayerSubsystem = GameInstance->GetSubsystem<USteamMultiplayerSubsystem>();
	}

	if(SteamMultiplayerSubsystem)
	{
		SteamMultiplayerSubsystem->SteamMultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		SteamMultiplayerSubsystem->SteamMultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		SteamMultiplayerSubsystem->SteamMultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		SteamMultiplayerSubsystem->SteamMultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		SteamMultiplayerSubsystem->SteamMultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

bool USteamMainMenu::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	if(HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}
	if(JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}
	
	return true;
}

void USteamMainMenu::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();
}

void USteamMainMenu::OnCreateSession(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if(World)
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		// if(GEngine)
		// {
		// 	GEngine->AddOnScreenDebugMessage(
		// 		-1,
		// 		15.f,
		// 		FColor::Yellow,
		// 		FString(TEXT("Failed to create session"))
		// 		);
		// }
		HostButton->SetIsEnabled(true);
	}
}

void USteamMainMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if(SteamMultiplayerSubsystem == nullptr)
	{
		return;
	}

	for(auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if(SettingsValue == MatchType)
		{
			SteamMultiplayerSubsystem->JoinSession(Result);
			return;
		}
	}
	if(!bWasSuccessful || SessionResults.Num() == 0)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void USteamMainMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if(Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if(PlayerController)
			{
				PlayerController->ClientTravel(Address, TRAVEL_Absolute);
			}
		}
	}

	if(Result != EOnJoinSessionCompleteResult::Success)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void USteamMainMenu::OnDestroySession(bool bWasSuccessful)
{
}

void USteamMainMenu::OnStartSession(bool bWasSuccessful)
{
}

void USteamMainMenu::HostButtonClicked()
{
	HostButton->SetIsEnabled(false);
	if(SteamMultiplayerSubsystem)
	{
		SteamMultiplayerSubsystem->CreateSession(4, FString("pinegreen"));
	}
}

void USteamMainMenu::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);
	if(SteamMultiplayerSubsystem)
	{
		SteamMultiplayerSubsystem->FindSessions(10000);
	}
}

void USteamMainMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
}
