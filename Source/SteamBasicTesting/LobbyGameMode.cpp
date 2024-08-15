// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "EnhancedInputSubsystems.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Character.h"
#include "SteamBasicTestingPlayerController.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(NewPlayer)
	{
		if(APawn* ExistingPawn = NewPlayer->GetPawn())
		{
			ExistingPawn->Destroy();
		}

		FVector SpawnLocation = FVector(1120.f,1320.f,92.f);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		ACharacter* NewCharacter = GetWorld()->SpawnActor<ACharacter>(DefaultPawnClass, SpawnLocation, SpawnRotation);
		if(NewCharacter)
		{
			NewPlayer->Possess(NewCharacter);

			UE_LOG(LogTemp, Warning, TEXT("Character Name : %s"), *NewCharacter->GetName());
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.f,
					FColor::Orange,
					FString(TEXT("Create Character"))
					);
			}
		}
		else
		{
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.f,
					FColor::Orange,
					FString(TEXT("Can't Create Character"))
					);
			}
		}
	}
	
	if(GameState)
	{
		int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				1,
				15.f,
				FColor::Yellow,
				FString::Printf(TEXT("Players in game : %d"), NumberOfPlayers)
				);
			UE_LOG(LogTemp, Warning, TEXT("Players in game"));

			APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
			if(PlayerState)
			{
				FString PlayerName = PlayerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(
					2,
					15.f,
					FColor::Cyan,
					FString::Printf(TEXT("%s has joined the game"), *PlayerName)
					);
			}
			
			UE_LOG(LogTemp, Warning, TEXT("joined the game"));
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>();
	if(PlayerState)
	{
		int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		GEngine->AddOnScreenDebugMessage(
			1,
			60.f,
			FColor::Yellow,
			FString::Printf(TEXT("Players in game : %d"), NumberOfPlayers - 1)
			);

		FString PlayerName = PlayerState->GetPlayerName();
		GEngine->AddOnScreenDebugMessage(
			2,
			60.f,
			FColor::Yellow,
			FString::Printf(TEXT("%s has exited the game"), *PlayerName)
			);
	}
}
