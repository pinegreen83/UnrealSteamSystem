// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionMenu.generated.h"

/**
 * 
 */
UCLASS()
class STEAMMULTIPLAYERBASIC_API USessionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("pinegreen")), FString LobbyPath = FString(TEXT("/Game/TopDown/Maps/TopDownMap")));

protected:

	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	/**
	 * SteamMultiplayerSessionSubsystem의 커스텀 delegate를 위한 callback 함수
	 */
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

private:
	UPROPERTY(meta=(BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta=(BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	/**
	 * 플레이어가 세션을 생성해서 월드를 이동하게 됐을 때 부르는 함수
	 * 위젯을 내리고 InputModeData를 변경.
	 */
	void MenuTearDown();

	// 모든 온라인 세션 기능을 다루기 위한 서브시스템
	class USteamMultiplayerSubsystem* SteamMultiplayerSubsystem;

	/**
	 * 세션 생성에 필요한 변수
	 * NumPublicConnection : 세션에 들어올 수 있는 인원 수
	 * MatchType : 세션 이름
	 * PathToLobby : 로비로 사용할 레벨의 경로를 저장
	 */
	int32 NumPublicConnections{4};
	FString MatchType{TEXT("pinegreen")};
	FString PathToLobby{TEXT("")};
};
