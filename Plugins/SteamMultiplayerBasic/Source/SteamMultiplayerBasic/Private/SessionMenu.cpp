// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionMenu.h"
#include "Components/Button.h"
#include "SteamMultiplayerSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void USessionMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch)
{
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

bool USessionMenu::Initialize()
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

void USessionMenu::NativeDestruct()
{
	Super::NativeDestruct();
}

void USessionMenu::OnCreateSession(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Yellow,
				FString(TEXT("Session Created Successfully"))
				);
		}
		
		UWorld* World = GetWorld();
		if(World)
		{
			World->ServerTravel("/Game/TopDown/Maps/TopDownMap?Listen");
		}
	}
	else
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Yellow,
				FString(TEXT("Failed to create session"))
				);
		}
	}
}

void USessionMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
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
}

void USessionMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
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
}

void USessionMenu::OnDestroySession(bool bWasSuccessful)
{
}

void USessionMenu::OnStartSession(bool bWasSuccessful)
{
}

void USessionMenu::HostButtonClicked()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Yellow,
				FString(TEXT("Host Button Clicked"))
			);
	}
	
	if(SteamMultiplayerSubsystem)
	{
		SteamMultiplayerSubsystem->CreateSession(4, FString("pinegreen"));
	}
}

void USessionMenu::JoinButtonClicked()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Yellow,
			FString(TEXT("Join Button Clicked"))
			);
	}

	if(SteamMultiplayerSubsystem)
	{
		SteamMultiplayerSubsystem->FindSessions(10000);
	}
}

void USessionMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			// PlayerController->SetShowMouseCursor(false);
		}
	}
}
