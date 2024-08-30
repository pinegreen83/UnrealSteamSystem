// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LocalMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class STEAMMULTIPLAYERBASIC_API ULocalMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void ToggleMenuElements(ESlateVisibility NewVisibility);
	
protected:
	virtual void NativeConstruct() override;
	
private:

	/*
	 * Main Menu에 사용될 버튼들
	 */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameTitle;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SinglePlayerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MultiplayerButton;

	UFUNCTION()
	void OnSinglePlayerButtonClicked();
	UFUNCTION()
	void OnMultiPlayerButtonClicked();

	
	/*
	 * 각자의 다른 기능을 하는 위젯들
	 */

	UPROPERTY(meta = (BindWidget))
	class ULocalMultiplayerMenu* MultiplayerMenu;
	
};
