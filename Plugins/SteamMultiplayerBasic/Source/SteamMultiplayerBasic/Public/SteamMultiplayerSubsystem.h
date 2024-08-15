// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "SteamMultiplayerSubsystem.generated.h"

/**
 * 메뉴 클래스에서 부를 콜백함수와 바인드된 delegate
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSteamMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FSteamMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FSteamMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSteamMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSteamMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class STEAMMULTIPLAYERBASIC_API USteamMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USteamMultiplayerSubsystem();

	// 세션 기능을 다루기 위한 함수들.
	/**
	 * CreateSession : NumPublicConnections = 몇 명의 플레이어들이 세션에 참여할 수 있는지, MatchType : 세션을 만들기 위해 사용.
	 */
	void CreateSession(int32 NumPublicConnections, FString MatchType);
	/**
	 * FindSessions : MaxSearchResults = 한 번에 몇 개의 세션을 찾을 것인지를 나타내는 함수
	 */
	void FindSessions(int32 MaxSearchResults);
	/**
	 * JoinSession : SessionResult = 검색한 세션들 중 참여하고 싶은 세션에 참여
	 */
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	void StartSession();
	
	/**
	 * 메뉴에서 세션을 생성했을 때 실행될 콜백함수를 바인드할 커스텀 delegate
	 */
	FSteamMultiplayerOnCreateSessionComplete SteamMultiplayerOnCreateSessionComplete;
	FSteamMultiplayerOnFindSessionsComplete SteamMultiplayerOnFindSessionsComplete;
	FSteamMultiplayerOnJoinSessionComplete SteamMultiplayerOnJoinSessionComplete;
	FSteamMultiplayerOnDestroySessionComplete SteamMultiplayerOnDestroySessionComplete;
	FSteamMultiplayerOnStartSessionComplete SteamMultiplayerOnStartSessionComplete;
	
protected:
	/**
	 * Online Session Interface의 Delegate list에 추가할 내부 callback 함수들
	 * SteamMultiplayerSubsystem이라는 이 클래스 외부에서 불릴 필요가 없음.
	 */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	auto OnDestroySessionComplete(FName SessionName, bool bWasSuccessful) -> void;
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	
private:
	IOnlineSessionPtr SessionInterface;
    TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	/**
	 * 세션 생성과 연관된 Delegate들
	 * SteamMultiplayerSubsystem의 내부 callback 함수와 바인딩할 것임.
	 */
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	/**
	 * 세션 생성 시 이미 존재하는 세션에 참가하려고 할 때 그 세션을 파괴하고 다시 만들 떄 필요한 변수
	 * bCreateSessionOnDestroy : 이미 존재하는 세션을 생성하고 파괴하려는 것인지 확인
	 * LastNumPublicConnections : 
	 */
	bool bCreateSessionOnDestroy{false};
	int32 LastNumPublicConnections;
	FString LastMatchType;
};
