// Fill out your copyright notice in the Description page of Project Settings.


#include "Steam/SteamCreateSessionWidget.h"
#include "OnlineSubsystem.h"
#include "SessionInfo.h"
#include "SteamMultiplayerSubsystem.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Steam/SteamMultiplayerMenu.h"

void USteamCreateSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(ReturnToMainButton)
	{
		ReturnToMainButton->OnClicked.AddDynamic(this, &USteamCreateSessionWidget::OnReturnToMainButtonClicked);
	}

	if(CreateSessionButton)
	{
		CreateSessionButton->OnClicked.AddDynamic(this, &USteamCreateSessionWidget::OnCreateSessionClicked);
	}
	
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		SteamMultiplayerSubsystem = GameInstance->GetSubsystem<USteamMultiplayerSubsystem>();
	}

	if(SteamMultiplayerSubsystem)
	{
		SteamMultiplayerSubsystem->SteamMultiplayerOnCreateSessionComplete.AddDynamic(this, &USteamCreateSessionWidget::OnCreateSessionComplete);
	}
}

void USteamCreateSessionWidget::OnCreateSessionClicked()
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
	SteamMultiplayerSubsystem->CreateSession(SessionInfo);
}

void USteamCreateSessionWidget::OnReturnToMainButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	
	if(MultiplayerMenu)
	{
		MultiplayerMenu->ToggleMenuElements(ESlateVisibility::Visible);
	}
}

void USteamCreateSessionWidget::OnCreateSessionComplete(const FName& SessionName, bool bWasSuccessful)
{
		if(bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Session created successfully."));
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString(TEXT("Session created successfully."))
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

void USteamCreateSessionWidget::InitializeMultiplayerMenu(TObjectPtr<class USteamMultiplayerMenu> InMultiplayerMenu)
{
	MultiplayerMenu = InMultiplayerMenu;
}
