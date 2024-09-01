// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SteamMenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class STEAMMULTIPLAYERBASIC_API ASteamMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASteamMenuGameModeBase();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USteamMainMenu> MainMenuWidgetClass;

	UPROPERTY()
	USteamMainMenu* MainMenuWidget;
};
