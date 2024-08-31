// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SteamCreateSessionWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEAMMULTIPLAYERBASIC_API USteamCreateSessionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 멀티플레이어 위젯의 포인터를 설정하는 함수.
	void InitializeMultiplayerMenu(TObjectPtr<class USteamMultiplayerMenu> InMultiplayerMenu);
	
protected:
	virtual void NativeConstruct() override;
	
private:
	/*
	 * 위젯에 필요한 요소들
	 */
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* MapSelectionComboBox;

	UPROPERTY(meta= (BindWidget))
	class USlider* MaxPlayerSlider;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* PrivateSessionCheckBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* SessionNameTextBox;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnToMainButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateSessionButton;

	UFUNCTION()
	void OnCreateSessionClicked();

	UFUNCTION()
	void OnReturnToMainButtonClicked();

	TObjectPtr<USteamMultiplayerMenu> MultiplayerMenu;

	/*
	 * 세션 생성에 필요한 요소들
	 */

	// 모든 온라인 세션 기능을 다루기 위한 서브시스템
	class USteamMultiplayerSubsystem* SteamMultiplayerSubsystem;

	UFUNCTION()
	void OnCreateSessionComplete(const FName& SessionName, bool bWasSuccessful);
};
