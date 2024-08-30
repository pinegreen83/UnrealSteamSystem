// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LocalMultiplayerMenu.generated.h"

/**
 * 
 */
UCLASS()
class STEAMMULTIPLAYERBASIC_API ULocalMultiplayerMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	// 위젯 초기화 함수
	void InitializeWidget();

	// 메인 메뉴의 포인터를 설정하는 함수
	void InitializeMainMenu(TObjectPtr<class ULocalMainMenu> InMainMenu);

protected:
	virtual void NativeConstruct() override;

private:
	TObjectPtr<ULocalMainMenu> MainMenuWidget;
	
	/*
	 * Multiplayer Menu에 사용될 위젯들
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnToMainButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* OpenCreateSessionPanelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FindSessionsButton;

	UFUNCTION()
	void OnReturnToMainButtonClicked();

	UFUNCTION()
	void OnOpenCreateSessionPanelButtonClicked();

	UFUNCTION()
	void OnFindSessionsButtonClicked();

};
