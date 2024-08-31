// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "LocalSessionInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEAMMULTIPLAYERBASIC_API ULocalSessionInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetSessionInfo(const FOnlineSessionSearchResult& SearchResult);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SessionNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SelectedMapText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerInRoomText;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinSessionButton;
	
	UFUNCTION()
	void OnJoinSessionButtonClicked();
	
	FOnlineSessionSearchResult StoredSessionResult;

	void JoinSession();
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

};
