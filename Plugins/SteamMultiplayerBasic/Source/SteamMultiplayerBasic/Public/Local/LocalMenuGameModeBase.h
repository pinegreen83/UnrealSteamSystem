// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LocalMenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class STEAMMULTIPLAYERBASIC_API ALocalMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALocalMenuGameModeBase();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class ULocalMainMenu> MainMenuWidgetClass;

	UPROPERTY()
	ULocalMainMenu* MainMenuWidget;
	
};
