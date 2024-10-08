// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LocalFindSessionsWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEAMMULTIPLAYERBASIC_API ULocalFindSessionsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 멀티플레이어 위젯의 포인터를 설정하는 함수.
	void InitializeMultiplayerMenu(TObjectPtr<class ULocalMultiplayerMenu> InMultiplayerMenu);

	void GetSessionSearchResult(TSharedPtr<class FOnlineSessionSearch> InSessionSearch);

	// 패널에 찾은 세션을 표시하기 위한 함수.
	void PopulateSessionList();

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ULocalSessionInfoWidget> LocalSessionInfoWidgetClass;
	
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
	TObjectPtr<ULocalMultiplayerMenu> MultiplayerMenu;
	
	// 세션을 목록으로 표시하기 위한 UI 요소
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* SessionPanelWidget;
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* SessionListScrollBox;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnToMainButton;
	
	UFUNCTION()
	void OnReturnToMainButtonClicked();
	
};
