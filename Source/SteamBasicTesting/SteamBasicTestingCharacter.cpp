// Copyright Epic Games, Inc. All Rights Reserved.

#include "SteamBasicTestingCharacter.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ASteamBasicTestingCharacter::ASteamBasicTestingCharacter():
    CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
		FriendsInterface = OnlineSubsystem->GetFriendsInterface();
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Found System %s"), *OnlineSubsystem->GetSubsystemName().ToString())
			);
		}
	}
}

void ASteamBasicTestingCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ASteamBasicTestingCharacter::CreateGameSession()
{
    // 키보드에서 1번 키를 누르면 실행
    // OnlineSessionInterface가 유효한지 확인.
    if(!OnlineSessionInterface.IsValid())
    {
    	return;
    }
    
    // OnlineSessionInterface가 유효하다면, 세션이 생성되어 있는지 확인
    auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
    if(ExistingSession != nullptr)
    {
    	// 만약 세션이 만들어져 있다면, 세션 파괴
    	OnlineSessionInterface->DestroySession(NAME_GameSession);
    }
    
    // Session Interface에 있는 Delegate list에 CreateSessionCompleteDelegate를 추가, 
    // 이렇게 하고 나면 Delegate와 묶인 OnCreateSessionComplete라는 Callback함수가 나중에 불림.
    OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
    
    // CreateSession() 함수를 실행하기 위한 설정.
    TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings);
    SessionSettings->bIsLANMatch = false;
    SessionSettings->NumPublicConnections = 4;
    SessionSettings->bAllowJoinInProgress = true;
    SessionSettings->bAllowJoinViaPresence = true;
    SessionSettings->bShouldAdvertise = true;
    SessionSettings->bUsesPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
    SessionSettings->Set(FName("MatchType"), FString("pinegreen"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}

void ASteamBasicTestingCharacter::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	// 세션 생성 성공 시
	if(bWasSuccessful)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Created session : %s"), *SessionName.ToString())
			);
		}
		MoveToLevel();
	}
	else
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session"))
			);
		}
	}
}

void ASteamBasicTestingCharacter::MoveToLevel()
{
	FName LevelName = "TopDownMap";
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void ASteamBasicTestingCharacter::GetFriendsList()
{
	if(FriendsInterface.IsValid())
	{
		FriendsInterface->ReadFriendsList(
		0, 
		EFriendsLists::ToString(EFriendsLists::Default),
		FOnReadFriendsListComplete::CreateUObject(this, &ASteamBasicTestingCharacter::OnReadFriendsListComplete)
		);
	}
}

void ASteamBasicTestingCharacter::OnReadFriendsListComplete(int32 LocalUserNum, bool bWasSuccessful,
	const FString& ListName, const FString& ErrorStr)
{
	if(bWasSuccessful)
	{
		if(FriendsInterface.IsValid())
		{
			TArray<TSharedRef<FOnlineFriend>> Friends;
			FriendsInterface->GetFriendsList(LocalUserNum, ListName, Friends);
			for(TSharedRef<FOnlineFriend> Friend : Friends)
			{
				if(GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,
						15.f,
						FColor::Red,
						FString::Printf(TEXT("친구 : %s (%s)"), *Friend->GetDisplayName(), *Friend->GetUserId()->ToString())
					);
				}
			}
		}
	}
}

void ASteamBasicTestingCharacter::InviteFriendsToSession(const FUniqueNetIdRepl& FriendUniqueNetId)
{
	
}


