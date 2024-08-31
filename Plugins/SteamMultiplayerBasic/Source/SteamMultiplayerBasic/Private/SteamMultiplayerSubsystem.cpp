// Fill out your copyright notice in the Description page of Project Settings.


#include "SteamMultiplayerSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "SessionInfo.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Online/OnlineSessionNames.h"

USteamMultiplayerSubsystem::USteamMultiplayerSubsystem():
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if(Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}
}

void USteamMultiplayerSubsystem::CreateSession(const FSessionInfo& SessionInfo)
{
	if(!SessionInterface.IsValid())
	{
		return;
	}

	// delegate를 delegate handle에 저장해서 추후에 delegate list에서 삭제가 가능하도록 만듦.
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
		LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "Null" ? true : false;
		LastSessionSettings->NumPublicConnections = SessionInfo.MaxPlayers;
		LastSessionSettings->bAllowJoinInProgress = true;
		LastSessionSettings->bAllowJoinViaPresence = true;
		LastSessionSettings->bShouldAdvertise = !SessionInfo.bIsPrivate;
		LastSessionSettings->bUsesPresence = true;
		LastSessionSettings->bAllowJoinInProgress = false;
		LastSessionSettings->bUseLobbiesIfAvailable = true;
		LastSessionSettings->BuildUniqueId = 1;

		// 커스텀 세션 정보 저장
		LastSessionSettings->Set(FName("DisplaySessionName"), SessionInfo.DisplaySessionName, EOnlineDataAdvertisementType::ViaOnlineService);
		LastSessionSettings->Set(FName("CreatorName"), SessionInfo.CreatorName, EOnlineDataAdvertisementType::ViaOnlineService);
		LastSessionSettings->Set(SETTING_MAPNAME, SessionInfo.SelectedMap, EOnlineDataAdvertisementType::ViaOnlineService);
	}
	
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if(!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		// 커스텀 delegate를 사용해서 broadcast
		SteamMultiplayerOnCreateSessionComplete.Broadcast("",false);
	}
}

void USteamMultiplayerSubsystem::FindSessions(int32 MaxSearchResults)
{
	if(!SessionInterface.IsValid())
	{
		return;
	}

	// delegate를 delegate handle에 저장해서 추후에 delegate list에서 삭제가 가능하도록 만듦.
	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "Null" ? true : false;
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if(!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		SteamMultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void USteamMultiplayerSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if(!SessionInterface.IsValid())
	{
		SteamMultiplayerOnJoinSessionComplete.Broadcast("", EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	FString JoinSession;
	SessionResult.Session.SessionSettings.Get(FName("SessionName"), JoinSession);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if(!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), *JoinSession, SessionResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		SteamMultiplayerOnJoinSessionComplete.Broadcast("", EOnJoinSessionCompleteResult::UnknownError);
	}
}

void USteamMultiplayerSubsystem::DestroySession()
{
	if(!SessionInterface.IsValid())
	{
		SteamMultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if(!SessionInterface->DestroySession(NAME_GameSession))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		SteamMultiplayerOnDestroySessionComplete.Broadcast(false);
	}
}

void USteamMultiplayerSubsystem::StartSession()
{
}

void USteamMultiplayerSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	SteamMultiplayerOnCreateSessionComplete.Broadcast(SessionName, bWasSuccessful);
}

void USteamMultiplayerSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if(SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if(LastSessionSearch->SearchResults.Num() <= 0)
	{
		SteamMultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	SteamMultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void USteamMultiplayerSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	SteamMultiplayerOnJoinSessionComplete.Broadcast(SessionName, Result);
}

auto USteamMultiplayerSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful) -> void
{
	if(SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}
	if(bWasSuccessful && bCreateSessionOnDestroy)
	{
		bCreateSessionOnDestroy = false;
	}
	SteamMultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

void USteamMultiplayerSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}
