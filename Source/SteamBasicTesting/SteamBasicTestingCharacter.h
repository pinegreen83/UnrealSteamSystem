// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "SteamBasicTestingCharacter.generated.h"

UCLASS(Blueprintable)
class ASteamBasicTestingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASteamBasicTestingCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	// 친구 목록을 가져오는 함수
	UFUNCTION(BlueprintCallable)
	void GetFriendsList();

	// 블루프린트나 다른 클래스에서 호출할 수 있는 함수
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
		
protected:
    
    // 세션 생성 완료 시 호출될 콜백 함수
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    
    // 세션이 성공적으로 생성되었을 경우 맵을 바꿔주는 함수
    void MoveToLevel();
                 
    // 친구 초대를 보내는 함수
    void InviteFriendsToSession(const FUniqueNetIdRepl& FriendUniqueNetId);
    
    // 친구 목록을 가져온 후 호출되는 함수
    void OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);
    
private:

    // Delegate와 Session Interface 관련 변수들
    FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
    IOnlineSessionPtr OnlineSessionInterface;
    IOnlineFriendsPtr FriendsInterface;
    
    // 첫 번째 친구의 UniqueNetId를 저장하기 위한 변수
    TSharedPtr<const FUniqueNetId> FirstFriendUniqueNetId;
};

