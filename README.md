# UnrealSteamSystem

**UnrealSteamSystem**은 언리얼 엔진에서 멀티플레이어 게임을 개발할 때 사용할 수 있는 스팀 멀티플레이어 세션 플러그인입니다. 이 플러그인은 스팀 플랫폼을 통해 멀티플레이어 세션을 관리하고, 쉽게 게임에 통합할 수 있도록 설계되었습니다. 또한, 스팀 환경이 아닌 로컬 환경에서 멀티플레이어 세션을 생성하고 테스트할 수 있는 기능을 제공합니다.

## 프로젝트 목적

이 플러그인은 멀티플레이어 게임 개발 시 스팀 세션 관리의 복잡성을 줄이고, 개발 시간을 단축하기 위해 제작되었습니다.
![방-예시](https://github.com/user-attachments/assets/9ce4bcd3-7a0d-4abd-8f6c-f61b915aece2)


## 기술 스택

이 프로젝트는 다음과 같은 기술과 도구를 사용하여 개발되었습니다:

- **언리얼 엔진**: 5.4.4 버전
- **Steam SDK**: 스팀 멀티플레이어 세션 관리를 위해 사용
- **C++**: 언리얼 엔진과의 통합 및 게임 로직 구현
- **UMG (Unreal Motion Graphics)**: 사용자 인터페이스(UI) 구성
- **Blueprint**: 일부 게임 로직 및 UI 이벤트 처리

## 주요 기능
- **스팀 세션 생성 및 관리**: 손쉽게 스팀 멀티플레이어 세션을 생성하고 관리할 수 있습니다.
![시연_방 생성](https://github.com/user-attachments/assets/2b9d2806-b569-45ba-b3e3-933f19e0c53a)

- **서버 브라우징**: 사용자가 참여 가능한 서버 목록을 쉽게 검색하고 표시할 수 있습니다.
![시연_방 참가_1](https://github.com/user-attachments/assets/ed425139-821e-4d8c-b464-787327d29dee)

- **로컬 멀티플레이어 테스트**: 스팀 환경 없이 로컬 환경에서 멀티플레이어 세션을 생성하고 테스트할 수 있는 기능을 제공합니다.
![시연_방 퇴장](https://github.com/user-attachments/assets/dc48bd94-b989-44df-976e-ad92458509c4)


- **간편한 통합**: 언리얼 엔진 프로젝트에 간편하게 통합할 수 있도록 설계되었습니다.

## 설치 방법
1. 이 레포지토리의 Plugins 폴더를 다운로드 합니다.
2.	언리얼 엔진 프로젝트의 Plugins 폴더에 다운로드한 폴더를 복사합니다. Plugins 폴더가 없다면, 생성한 후 복사하세요.
3.	필요에 따라 플러그인을 설정하고, 게임에 맞게 조정합니다.

## 사용 방법
이 플러그인은 스팀 멀티플레이어 기능을 빠르게 설정할 수 있도록 구성되어 있습니다. 
기본적인 사용 방법은 다음과 같습니다.

	1. 세션 생성 : 새로운 멀티플레이어 세션을 생성하려면, 플러그인의 API를 사용하여 세션을 시작할 수 있습니다.
	2. 서버 브라우징 : 기존의 멀티플레이어 세션을 찾고 참가하려면, 서버 브라우징 기능을 활용하세요.
	3. 세션 관리 : 세션 중지 및 관리는 플러그인의 내장 기능을 통해 쉽게 수행할 수 있습니다.

    ## 프로젝트 아키텍처

UnrealSteamSystem은 다음과 같은 주요 구성 요소로 이루어져 있습니다:

- **SteamMultiplayerSubsystem**: 언리얼 엔진에서 스팀을 활용한 멀티플레이어 세션 관리 기능을 제공하는 핵심 서브시스템입니다.
- **SteamCreateSessionWidget**: 멀티플레이어 세션을 생성하기 위한 UI를 제공합니다. 이 클래스는 세션 생성 시 필요한 정보를 입력받고, 세션 생성을 처리하는 역할을 합니다.
- **SteamFindSessionsWidget**: 사용자가 멀티플레이어 세션을 검색하고, 검색된 세션들을 UI에 표시하는 역할을 담당하는 위젯입니다. 이 클래스는 세션 목록을 표시하고, 세션에 대한 정보를 시각적으로 제공하며, 사용자가 선택한 세션에 쉽게 접근할 수 있도록 합니다.

## 주요 클래스 및 메서드 설명

### 1. SteamMultiplayerSubsystem

- **세션 생성 (`CreateSession`)**: 주어진 세션 정보를 바탕으로 새로운 멀티플레이어 세션을 생성합니다. 세션 생성 완료 시, 결과를 알리는 델리게이트를 호출합니다.
  
- **세션 검색 (`FindSessions`)**: 설정된 검색 조건에 따라 현재 사용 가능한 세션들을 검색합니다. 검색 완료 후, 결과를 알리는 델리게이트를 호출합니다.
  
- **세션 참가 (`JoinSession`)**: 검색된 세션 중 하나에 참가합니다. 세션 참가 완료 시, 결과를 알리는 델리게이트를 호출합니다.

#### 주요 델리게이트

- **`FOnCreateSessionCompleteDelegate`**: 세션 생성이 완료되었을 때 호출됩니다.
- **`FOnFindSessionsCompleteDelegate`**: 세션 검색이 완료되었을 때 호출됩니다.
- **`FOnJoinSessionCompleteDelegate`**: 세션 참가가 완료되었을 때 호출됩니다.
- **`FOnDestroySessionCompleteDelegate`**: 세션 종료가 완료되었을 때 호출됩니다.
- **`FOnStartSessionCompleteDelegate`**: 세션 시작이 완료되었을 때 호출됩니다.

#### 클래스 사용 예시

```cpp
USteamMultiplayerSubsystem* MultiplayerSubsystem = NewObject<USteamMultiplayerSubsystem>();

// 세션 생성
FSessionInfo SessionInfo;
SessionInfo.MaxPlayers = 10;
SessionInfo.DisplaySessionName = "MyGameSession";
SessionInfo.CreatorName = "PlayerOne";
SessionInfo.SelectedMap = "MapName";
MultiplayerSubsystem->CreateSession(SessionInfo);

// 세션 검색
MultiplayerSubsystem->FindSessions(10);

// 세션 참가
FOnlineSessionSearchResult SearchResult;
MultiplayerSubsystem->JoinSession(SearchResult);

// 세션 종료
MultiplayerSubsystem->DestroySession();
```
https://github.com/pinegreen83/UnrealSteamSystem/blob/8ddd3760ed8b054f0466247e5fb05e70fa8faf47/Plugins/SteamMultiplayerBasic/Source/SteamMultiplayerBasic/Private/SteamMultiplayerSubsystem.cpp

### 2. SteamCreateSessionWidget

- **세션 생성 (`OnCreateSessionClicked`)**: 사용자가 입력한 정보(맵 선택, 최대 플레이어 수, 세션 이름 등)를 바탕으로 새로운 멀티플레이어 세션을 생성합니다. 세션 생성 요청 후, 결과에 따라 성공 또는 실패 메시지를 표시합니다.
  
- **메인 메뉴로 돌아가기 (`OnReturnToMainButtonClicked`)**: 사용자가 메인 메뉴로 돌아가고자 할 때, 위젯의 가시성을 숨기고 메인 메뉴의 UI 요소를 다시 표시합니다.

- **세션 생성 완료 처리 (`OnCreateSessionComplete`)**: 세션 생성이 완료되었을 때 호출되며, 성공 시 선택된 맵으로 레벨을 로드합니다.

#### 주요 위젯 요소

- **`ReturnToMainButton`**: 메인 메뉴로 돌아가기 위한 버튼입니다.
- **`CreateSessionButton`**: 사용자가 세션 생성을 시작할 수 있는 버튼입니다.
- **`MapSelectionComboBox`**: 사용자가 플레이할 맵을 선택할 수 있는 콤보박스입니다.
- **`MaxPlayerSlider`**: 방에 참여할 수 있는 최대 플레이어 수를 설정하는 슬라이더입니다.
- **`PrivateSessionCheckBox`**: 세션을 비공개로 설정할지 여부를 결정하는 체크박스입니다.
- **`SessionNameTextBox`**: 사용자가 설정한 세션 이름을 입력하는 텍스트 박스입니다.

#### 클래스 사용 예시

```cpp
// 위젯 생성 후 초기화
USteamCreateSessionWidget* CreateSessionWidget = CreateWidget<USteamCreateSessionWidget>(GetWorld(), USteamCreateSessionWidget::StaticClass());
CreateSessionWidget->AddToViewport();

// 멀티플레이어 메뉴와 연동
CreateSessionWidget->InitializeMultiplayerMenu(MultiplayerMenu);

// 세션 생성 버튼 클릭 시 호출될 메서드 바인딩
CreateSessionWidget->CreateSessionButton->OnClicked.AddDynamic(this, &USteamCreateSessionWidget::OnCreateSessionClicked);
```
https://github.com/pinegreen83/UnrealSteamSystem/blob/8ddd3760ed8b054f0466247e5fb05e70fa8faf47/Plugins/SteamMultiplayerBasic/Source/SteamMultiplayerBasic/Private/Steam/SteamCreateSessionWidget.cpp

### 3. SteamFindSessionsWidget

- **세션 검색 시작 (`StartFindSessions`)**: 멀티플레이어 세션 검색을 시작합니다. 최대 검색 결과 수를 설정하고, 세션 검색 요청을 보냅니다.

- **세션 검색 완료 처리 (`OnFindSessionsComplete`)**: 세션 검색이 완료되면, 검색 결과를 처리하고 UI에 표시합니다. 검색 결과가 없을 경우 경고 메시지를 표시합니다.

- **세션 목록 채우기 (`PopulateSessionList`)**: 검색된 세션 목록을 스크롤 박스(`ScrollBox`)에 동적으로 추가합니다. 각 세션은 `USteamSessionInfoWidget`을 통해 표시됩니다.

- **메인 메뉴로 돌아가기 (`OnReturnToMainButtonClicked`)**: 사용자가 메인 메뉴로 돌아가고자 할 때, 위젯의 가시성을 숨기고 메인 메뉴의 UI 요소를 다시 표시합니다.

#### 주요 위젯 요소

- **`ReturnToMainButton`**: 메인 메뉴로 돌아가기 위한 버튼입니다.
- **`SessionListScrollBox`**: 검색된 세션들을 표시할 스크롤 박스입니다.
- **`SteamSessionInfoWidgetClass`**: 개별 세션 정보를 표시하기 위한 위젯 클래스입니다.

#### 클래스 사용 예시

```cpp
// 위젯 생성 후 초기화
USteamFindSessionsWidget* FindSessionsWidget = CreateWidget<USteamFindSessionsWidget>(GetWorld(), USteamFindSessionsWidget::StaticClass());
FindSessionsWidget->AddToViewport();

// 멀티플레이어 메뉴와 연동
FindSessionsWidget->InitializeMultiplayerMenu(MultiplayerMenu);

// 세션 검색 시작
FindSessionsWidget->StartFindSessions();
```

https://github.com/pinegreen83/UnrealSteamSystem/blob/8ddd3760ed8b054f0466247e5fb05e70fa8faf47/Plugins/SteamMultiplayerBasic/Source/SteamMultiplayerBasic/Private/Steam/SteamFindSessionsWidget.cpp

## 프로젝트 요약 및 혜택

**UnrealSteamSystem**을 사용하면, 스팀 멀티플레이어 세션의 복잡한 구현을 신속하고 간단하게 처리할 수 있습니다. 이 플러그인은 멀티플레이어 게임의 핵심 기능인 세션 생성, 서버 브라우징, 그리고 세션 관리를 손쉽게 구현할 수 있도록 설계되었습니다.

### 이 플러그인의 주요 혜택:
- **시간 절약**: 스팀 멀티플레이어 시스템의 복잡한 부분을 직접 구현할 필요 없이, 이 플러그인을 사용하여 빠르게 설정할 수 있습니다.
- **효율적인 개발**: 멀티플레이어 기능 구현에 필요한 리소스를 절약하고, 게임의 콘텐츠와 인게임 기능에 집중할 수 있습니다.
- **로컬 및 스팀 환경 모두 지원**: 스팀 환경뿐만 아니라 로컬 환경에서도 멀티플레이어 기능을 테스트할 수 있어, 다양한 개발 시나리오에서 유연하게 사용할 수 있습니다.

이 플러그인을 사용하면, 멀티플레이어 기능에 소요되는 시간을 줄이고, 게임 개발의 핵심적인 부분에 더 많은 시간을 할애할 수 있습니다.