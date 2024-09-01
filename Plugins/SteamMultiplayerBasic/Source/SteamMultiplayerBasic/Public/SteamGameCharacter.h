// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SteamGameCharacter.generated.h"

struct FInputActionValue;

UCLASS()
class STEAMMULTIPLAYERBASIC_API ASteamGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASteamGameCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/*
	 * Camera Section
	 * SpringArm : 카메라를 지탱해주는 지지대의 역할을 함.
	 * CameraComponent : 실제 카메라의 역할을 하는 컴포넌트.
	 * "meta" <= 현재 private로 설정된 언리얼 오브젝트의 객체들을 블루프린트에서도 접근할 수 있도록 만들어주는 지시자.
	 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;


	// Input Mapping Contexts
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> GamePlayMappingContext;
	
	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MenuAction;
	
private:
	// 위젯 클래스 저장
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UInGameWidget> InGameWidgetClass;

	TObjectPtr<UInGameWidget> InGameWidget;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	bool bIsMenuOpen = false;

	void Menu();
	void SetInputModeForGameplay();
	void SetInputModeForUI();
	
};
