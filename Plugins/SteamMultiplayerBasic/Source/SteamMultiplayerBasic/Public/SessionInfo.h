// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SessionInfo.generated.h"

USTRUCT(BlueprintType)
struct STEAMMULTIPLAYERBASIC_API FSessionInfo
{
	GENERATED_BODY()

public:
	// ==============================
	// 생성된 세션에 대한 정보를 담는 구조체
	// ==============================
	UPROPERTY(BlueprintReadWrite)
	FString SessionName;

	UPROPERTY(BlueprintReadWrite)
	FString DisplaySessionName;

	UPROPERTY(BlueprintReadWrite)
	FString CreatorName;

	UPROPERTY(BlueprintReadWrite)
	FString SelectedMap;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadWrite)
	bool bIsPrivate;
};