// Fill out your copyright notice in the Description page of Project Settings.


#include "SteamGameCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InGameWidget.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASteamGameCharacter::ASteamGameCharacter()
{
	/*
	 * Pawn의 기본설정
	 * 그 중에서도 회전을 지정하기 위해서 기본적으로 세팅하는 값들.
	 */
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	/*
	 * Capsule의 기본설정
	 * 캡슐은 실제로 구현을 해야 되기 때문에 헤더를 추가
	 */
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	/*
	 * Movement의 기본설정
	 * 움직임에 대한 설정들임.
	 */
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	
	/*
	 * SpringArm의 기본 설정
	 * 생성을 한 뒤, 루트 컴포넌트에 부착후 길이를 4m로 지정.
	 * 회전에 대한 설정 추가.
	 */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	/*
	 * Camera의 기본 설정
	 * 카메라도 마찬가지로 생성을 한 뒤, 이번엔 스프링 암에 부착함.
	 * 부착을 할 때는 특정 위치를 지정하는 것이 아닌, 소켓이라는 이름 지시자를 지정하면
	 * 스프링 암의 끝에 자동으로 달라붙음.
	 */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// 메뉴 위젯 기본 설정
	static ConstructorHelpers::FClassFinder<UInGameWidget> MenuWidgetClass(TEXT("/SteamMultiplayerBasic/UI/WBP_InGame.WBP_InGame_C"));
	if (MenuWidgetClass.Succeeded())
	{
		InGameWidgetClass = MenuWidgetClass.Class;
	}
	
	// Input 기본 설정
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/SteamMultiplayerBasic/Input/IMC_GamePlay.IMC_GamePlay'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		GamePlayMappingContext = InputMappingContextRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/SteamMultiplayerBasic/Input/Actions/IA_Jump.IA_Jump'"));
	if(nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/SteamMultiplayerBasic/Input/Actions/IA_Move.IA_Move'"));
	if(nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/SteamMultiplayerBasic/Input/Actions/IA_Look.IA_Look'"));
	if(nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMenuRef(TEXT("/Script/EnhancedInput.InputAction'/SteamMultiplayerBasic/Input/Actions/IA_Menu.IA_Menu'"));
	if(nullptr != InputActionMenuRef.Object)
	{
		MenuAction = InputActionMenuRef.Object;
	}
}

// Called when the game starts or when spawned
void ASteamGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(GamePlayMappingContext, 0);
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				30.f,
				FColor::Blue,
				FString::Printf(TEXT("Now Mapping Context : %s"), *GamePlayMappingContext->GetName()));
		}
	}
}

// Called to bind functionality to input
void ASteamGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASteamGameCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASteamGameCharacter::Look);
	EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Triggered, this, &ASteamGameCharacter::Menu);
}

void ASteamGameCharacter::Move(const FInputActionValue& Value)
{
	/*
	 * InputActionValue에서 X와 Y값을 가져온 후,
	 * 무브먼트 컴포넌트와 연결해서 실질적으로 캐릭터를 이동시킴.
	 */
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ASteamGameCharacter::Look(const FInputActionValue& Value)
{
	/*
	 * InputActionValue에서 X와 Y값을 가져온 후,
	 * 컨트롤러의 회전을 설정해서 스프링 암이 해당 컨트롤러를 바라보도록 설정함.
	 */
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.Y);
	AddControllerPitchInput(LookAxisVector.X);
}

void ASteamGameCharacter::Menu()
{
	if (bIsMenuOpen)
	{
		if(InGameWidget)
		{
			InGameWidget->Teardown();
		}
		// 메뉴 닫기: 게임 플레이로 돌아가기
		SetInputModeForGameplay();
		bIsMenuOpen = false;
	}
	else
	{
		if(!InGameWidget && InGameWidgetClass)
		{
			InGameWidget = CreateWidget<UInGameWidget>(GetWorld(), InGameWidgetClass);
		}
		if(InGameWidget)
		{
			InGameWidget->SetUp();
		}
		// 메뉴 열기: UI 모드로 전환
		SetInputModeForUI();
		bIsMenuOpen = true;
	}
}

void ASteamGameCharacter::SetInputModeForGameplay()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);
	}
}

void ASteamGameCharacter::SetInputModeForUI()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
}

