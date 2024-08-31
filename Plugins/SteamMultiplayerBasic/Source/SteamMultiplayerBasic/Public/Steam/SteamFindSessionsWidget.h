// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SteamFindSessionsWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEAMMULTIPLAYERBASIC_API USteamFindSessionsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 멀티플레이어 위젯의 포인터를 설정하는 함수.
	void InitializeMultiplayerMenu(TObjectPtr<class USteamMultiplayerMenu> InMultiplayerMenu);

	// 멀티에서 넘어올 때 세션 찾는 함수.
	void StartFindSessions();

	// 패널에 찾은 세션을 표시하기 위한 함수.
	void PopulateSessionList(const TArray<FOnlineSessionSearchResult>& SessionResults);

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USteamSessionInfoWidget> SteamSessionInfoWidgetClass;
	
	TObjectPtr<USteamMultiplayerMenu> MultiplayerMenu;
	
	// 세션을 목록으로 표시하기 위한 UI 요소
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* SessionPanelWidget;
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* SessionListScrollBox;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnToMainButton;
	
	// 모든 온라인 세션 기능을 다루기 위한 서브시스템
	class USteamMultiplayerSubsystem* SteamMultiplayerSubsystem;
	
	IOnlineSessionPtr SessionInterface;;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
	
	UFUNCTION()
	void OnReturnToMainButtonClicked();
	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	
};
