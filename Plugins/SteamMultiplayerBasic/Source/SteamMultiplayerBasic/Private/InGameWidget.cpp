// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"

void UInGameWidget::SetUp()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
}

void UInGameWidget::Teardown()
{
	RemoveFromParent();
}
