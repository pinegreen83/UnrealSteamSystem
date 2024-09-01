// Fill out your copyright notice in the Description page of Project Settings.


#include "Local/LocalMainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Local/LocalMultiplayerMenu.h"

void ULocalMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	MultiplayerMenu->SetVisibility(ESlateVisibility::Hidden);
	
	if(SinglePlayerButton)
	{
		SinglePlayerButton->OnClicked.AddDynamic(this, &ULocalMainMenu::OnSinglePlayerButtonClicked);
	}

	if(MultiplayerButton)
	{
		MultiplayerButton->OnClicked.AddDynamic(this, &ULocalMainMenu::OnMultiPlayerButtonClicked);
	}
	
	if(QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &ULocalMainMenu::OnQuitGameButtonClicked);
	}
}

void ULocalMainMenu::OnSinglePlayerButtonClicked()
{
	ToggleMenuElements(ESlateVisibility::Hidden);

	FString LevelName = "SinglePlayLevel";
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}

void ULocalMainMenu::OnMultiPlayerButtonClicked()
{
	ToggleMenuElements(ESlateVisibility::Hidden);

	if(MultiplayerMenu)
	{
		MultiplayerMenu->InitializeMainMenu(this);
		MultiplayerMenu->SetVisibility(ESlateVisibility::Visible);
		MultiplayerMenu->InitializeWidget();
	}
}

void ULocalMainMenu::OnQuitGameButtonClicked()
{
	// 에디터 환경인지 확인 후, 에디터에서는 종료하지 않음
	if (!GEngine->IsEditor())
	{
		FGenericPlatformMisc::RequestExit(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("게임 종료는 에디터에서는 실행되지 않습니다."));
	}
}

void ULocalMainMenu::ToggleMenuElements(ESlateVisibility NewVisibility)
{
	// 메인 메뉴에 있는 위젯들 숨기기.
	GameTitle->SetVisibility(NewVisibility);
	SinglePlayerButton->SetVisibility(NewVisibility);
	MultiplayerButton->SetVisibility(NewVisibility);
}
