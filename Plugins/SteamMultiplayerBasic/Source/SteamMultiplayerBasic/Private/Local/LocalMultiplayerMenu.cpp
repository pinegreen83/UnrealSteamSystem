// Fill out your copyright notice in the Description page of Project Settings.


#include "Local/LocalMultiplayerMenu.h"
#include "Components/Button.h"
#include "Local/LocalMainMenu.h"

void ULocalMultiplayerMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(ReturnToMainButton)
	{
		ReturnToMainButton->OnClicked.AddDynamic(this, &ULocalMultiplayerMenu::OnReturnToMainButtonClicked);
	}

	if(OpenCreateSessionPanelButton)
	{
		OpenCreateSessionPanelButton->OnClicked.AddDynamic(this, &ULocalMultiplayerMenu::OnOpenCreateSessionPanelButtonClicked);
	}

	if(FindSessionsButton)
	{
		FindSessionsButton->OnClicked.AddDynamic(this, &ULocalMultiplayerMenu::OnFindSessionsButtonClicked);
	}
}

/*
 * 버튼에 대한 동작 함수들
 */

void ULocalMultiplayerMenu::OnReturnToMainButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);

	if (MainMenuWidget)
    {
        MainMenuWidget->ToggleMenuElements(ESlateVisibility::Visible);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MainMenuWidget is null in OnReturnToMainButtonClicked"));
    }
}

void ULocalMultiplayerMenu::OnOpenCreateSessionPanelButtonClicked()
{
	
}

void ULocalMultiplayerMenu::OnFindSessionsButtonClicked()
{
	
}

// 멀티플레이 메뉴가 메인 메뉴에서 AddToViewport 이전에 버튼등 요소 초기화 함수
void ULocalMultiplayerMenu::InitializeWidget()
{
	
}

// 멀티플레이 함수에서 메인 메뉴로 다시 돌아가기 위한 포인터를 받아오는 함수
void ULocalMultiplayerMenu::InitializeMainMenu(TObjectPtr<class ULocalMainMenu> InMainMenu)
{
	MainMenuWidget = InMainMenu;
}
