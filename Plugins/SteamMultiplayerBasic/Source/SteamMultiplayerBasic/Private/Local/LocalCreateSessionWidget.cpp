// Fill out your copyright notice in the Description page of Project Settings.


#include "Local/LocalCreateSessionWidget.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Local/LocalMultiplayerMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

void ULocalCreateSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(ReturnToMainButton)
	{
		ReturnToMainButton->OnClicked.AddDynamic(this, &ULocalCreateSessionWidget::OnReturnToMainButtonClicked);
	}

	if(CreateSessionButton)
	{
		CreateSessionButton->OnClicked.AddDynamic(this, &ULocalCreateSessionWidget::OnCreateSessionClicked);
	}
}

void ULocalCreateSessionWidget::OnCreateSessionClicked()
{
	/*
	 * SessionInfo : 방 생성에 필요한 정보를 담은 구조체
	 * SelectedMap : 사용자가 선택한 맵
	 * MaxPlayers : 방에 들어올 수 있는 최대 플레이어 수
	 * bIsPrivate : 방을 비공개 방으로 만들것인지 아닌지
	 * DisplaySessionName : 사용자가 설정하고 싶은 방 이름
	 * CreatorName : 방을 만든 플레이어의 이름
	 */
	FSessionInfo SessionInfo;
	SessionInfo.SelectedMap = MapSelectionComboBox->GetSelectedOption();
	SessionInfo.MaxPlayers = FMath::RoundToInt(MaxPlayerSlider->GetValue() * 1.f);
	SessionInfo.bIsPrivate = PrivateSessionCheckBox->IsChecked();
	SessionInfo.SessionName = SessionNameTextBox->GetText().ToString();
	SessionInfo.DisplaySessionName = SessionInfo.SessionName;
	
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Cyan,
			FString::Printf(TEXT("Session Name: %d"), SessionInfo.MaxPlayers));
	}

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		if(const IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface(); Identity.IsValid())
		{
			if(const FUniqueNetIdRepl PlayerUniqueNetId = Identity->GetUniquePlayerId(0); PlayerUniqueNetId.IsValid())
			{
				SessionInfo.CreatorName = Identity->GetPlayerNickname(*PlayerUniqueNetId);
				SessionInfo.SessionName += FString::Printf(TEXT("_%s"), *SessionInfo.CreatorName);
			}
		}
	}

	// 세션 생성
	CreateSession(SessionInfo);
}

void ULocalCreateSessionWidget::OnReturnToMainButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	
	if(MultiplayerMenu)
	{
		MultiplayerMenu->ToggleMenuElements(ESlateVisibility::Visible);
	}
}

void ULocalCreateSessionWidget::CreateSession(const FSessionInfo& SessionInfo)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
		if(Sessions.IsValid())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsLANMatch = true;
			SessionSettings.NumPublicConnections = SessionInfo.MaxPlayers;
			SessionSettings.bShouldAdvertise = !SessionInfo.bIsPrivate;
			SessionSettings.bUsesPresence = true;
			SessionSettings.bAllowJoinInProgress = false;

			// 커스텀 세션 정보 저장
			SessionSettings.Set(FName("DisplaySessionName"), SessionInfo.DisplaySessionName, EOnlineDataAdvertisementType::ViaOnlineService);
			SessionSettings.Set(FName("CreatorName"), SessionInfo.CreatorName, EOnlineDataAdvertisementType::ViaOnlineService);
			SessionSettings.Set(SETTING_MAPNAME, SessionInfo.SelectedMap, EOnlineDataAdvertisementType::ViaOnlineService);
			
			Sessions->OnCreateSessionCompleteDelegates.AddUObject(this, &ULocalCreateSessionWidget::OnCreateSessionComplete);
			Sessions->CreateSession(0, FName(*SessionInfo.SessionName), SessionSettings);
		}
	}
}

void ULocalCreateSessionWidget::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Session '%s' created successfully."), *SessionName.ToString());
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString::Printf(TEXT("Session '%s' created successfully."), *SessionName.ToString())
				);
		}
		FString LevelName = "SinglePlayLevel";
		UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), true, "listen");
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to create session."));
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString(TEXT("Failed to create session."))
				);
		}
	}
}

void ULocalCreateSessionWidget::InitializeMultiplayerMenu(TObjectPtr<class ULocalMultiplayerMenu> InMultiplayerMenu)
{
	MultiplayerMenu = InMultiplayerMenu;
}