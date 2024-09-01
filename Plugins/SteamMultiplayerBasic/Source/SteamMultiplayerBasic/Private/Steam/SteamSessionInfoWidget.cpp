// Fill out your copyright notice in the Description page of Project Settings.


#include "Steam/SteamSessionInfoWidget.h"

#include "SteamMultiplayerSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Online/OnlineSessionNames.h"

void USteamSessionInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(JoinSessionButton)
	{
		JoinSessionButton->OnClicked.AddDynamic(this, &USteamSessionInfoWidget::OnJoinSessionButtonClicked);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		SteamMultiplayerSubsystem = GameInstance->GetSubsystem<USteamMultiplayerSubsystem>();
	}

	if(SteamMultiplayerSubsystem)
	{
		SteamMultiplayerSubsystem->SteamMultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSessionComplete);
	}
}

void USteamSessionInfoWidget::SetSessionInfo(const FOnlineSessionSearchResult& SearchResult)
{
	StoredSessionResult = SearchResult;

	// 방을 찾는 플레이어에게 보여줄 방 이름
	FString DisplaySessionName;
	if(SearchResult.Session.SessionSettings.Get(FName("DisplaySessionName"), DisplaySessionName))
	{
		SessionNameText->SetText(FText::FromString(DisplaySessionName));
		UE_LOG(LogTemp, Log, TEXT("Session Name: %s"), *DisplaySessionName);
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString::Printf(TEXT("Session Name: %s"), *DisplaySessionName));
		}
	}

	// 방을 만든 플레이어가 선택한 맵 이름
	FString SelectedMap;
	if (SearchResult.Session.SessionSettings.Get(SETTING_MAPNAME, SelectedMap))
	{
		SelectedMapText->SetText(FText::FromString(SelectedMap));
		UE_LOG(LogTemp, Log, TEXT("Selected Map: %s"), *SelectedMap);
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString::Printf(TEXT("Selected Map: %s"), *SelectedMap));
		}
	}
	UE_LOG(LogTemp, Log, TEXT("NumPublicConnections: %d, NumOpenPublicConnections: %d"),
						SearchResult.Session.SessionSettings.NumPublicConnections,
						SearchResult.Session.NumOpenPublicConnections);
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Cyan,
			FString::Printf(TEXT("NumPublicConnections: %d, NumOpenPublicConnections: %d"),
						SearchResult.Session.SessionSettings.NumPublicConnections,
						SearchResult.Session.NumOpenPublicConnections)
			);
	}

	// 방을 생성한 플레이어의 이름
	FString RoomHostName;
	if(SearchResult.Session.SessionSettings.Get(FName("CreatorName"), RoomHostName))
	{
		RoomHostNameText->SetText(FText::FromString(RoomHostName));
		UE_LOG(LogTemp, Log, TEXT("CreatorName : %s"), *RoomHostName);
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString::Printf(TEXT("CreatorName : %s"), *RoomHostName));
		}
	}
	
	// 현재 방에 있는 플레이어의 수 / 최대 수용가능한 플레이어 수
	// 최대 플레이어 수
	int32 MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
	// 현재 플레이어 수
	int32 CurrentPlayers = MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
	FString PlayerInRoom = FString::Printf(TEXT("%d/%d"), CurrentPlayers, MaxPlayers);
	PlayerInRoomText->SetText(FText::FromString(PlayerInRoom));
	UE_LOG(LogTemp, Log, TEXT("PlayerInRoom : %s"), *PlayerInRoom);
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Cyan,
			FString::Printf(TEXT("PlayerInRoom : %s"), *PlayerInRoom));
	}
}

void USteamSessionInfoWidget::OnJoinSessionButtonClicked()
{
	JoinSessionButton->SetIsEnabled(false);
	if(SteamMultiplayerSubsystem)
	{
		SteamMultiplayerSubsystem->JoinSession(StoredSessionResult);
	}
}

void USteamSessionInfoWidget::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
		if(OnlineSubsystem)
		{
			IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
			if(Sessions.IsValid())
			{
				FString ConnectString;
				if(Sessions->GetResolvedConnectString(SessionName, ConnectString))
				{
					UE_LOG(LogTemp, Log, TEXT("Joining Session %s"), *ConnectString);
					if(GEngine)
					{
						GEngine->AddOnScreenDebugMessage(
							-1,
							15.f,
							FColor::Cyan,
							FString::Printf(TEXT("Joining Session %s"), *ConnectString)
							);
					}
					APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
					if(PlayerController)
					{
						PlayerController->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Join Failed"));
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString(TEXT("Join Failed"))
				);
		}
	}
}
