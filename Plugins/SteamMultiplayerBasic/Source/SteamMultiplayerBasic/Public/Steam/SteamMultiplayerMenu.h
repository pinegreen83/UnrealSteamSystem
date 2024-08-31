// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SteamMultiplayerMenu.generated.h"

/**
 * 
 */
UCLASS()
class STEAMMULTIPLAYERBASIC_API USteamMultiplayerMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	// 멀티플레이어 메뉴를 열 때 요소들을 초기화하는 함수.
	void InitializeWidget();

	// 요소들을 켜놓을 건지 꺼놓을 건지를 한번에 설정하는 함수.
	void ToggleMenuElements(ESlateVisibility NewVisibility);
	
	// 메인 메뉴의 포인터를 설정하는 함수
	void InitializeMainMenu(TObjectPtr<class USteamMainMenu> InMainMenu);

protected:
	virtual void NativeConstruct() override;

private:
	/*
	 * 다른 위젯들에 대한 정보
	 */
	TObjectPtr<USteamMainMenu> MainMenuWidget;
	
	UPROPERTY(meta = (BindWidget))
	class USteamCreateSessionWidget* CreateSessionWidget;

	UPROPERTY(meta = (BindWidget))
	class USteamFindSessionsWidget* FindSessionsWidget;
	
	/*
	 * Multiplayer Menu에 사용될 위젯들
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnToMainButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* OpenCreateSessionPanelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OpenFindSessionsPanelButton;

	UFUNCTION()
	void OnReturnToMainButtonClicked();
	
	/*
	 * 세션 생성에 사용될 함수들
	*/
	UFUNCTION()
	void OnOpenCreateSessionPanelButtonClicked();

	/*
	 * 세션 검색에 사용될 함수들
	 */
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	
	UFUNCTION()
	void OnOpenFindSessionsPanelButtonClicked();
	
};
