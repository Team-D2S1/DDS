// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::OnCreateSession(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		// TODO
		// MainServer에 CreateSession 요청 보낸다
		// MainServer가 포트 번호 보내준다면 해당 아이피:포트 로 이동한다
	}
}

void UMainMenuWidget::OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccessful)
{
	
}

void UMainMenuWidget::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	
}

void UMainMenuWidget::OnDestroySession(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		// TODO
		// MainServer에 DestroySession 요청 보낸다
		// 종료 전 작업 수행하고 Dedicated Server 종료
		// 이후 Port 반납
	}
}

bool UMainMenuWidget::Initialize()
{
	if(!Super::Initialize()) return false;
	
	SingleplayButton->OnClicked.AddDynamic(this, &ThisClass::SingleplayButtonClicked);
	MultiplayButton->OnClicked.AddDynamic(this, &ThisClass::MultiplayButtonClicked);
	OptionButton->OnClicked.AddDynamic(this, &ThisClass::OptionButtonClicked);
	ExitButton->OnClicked.AddDynamic(this, &ThisClass::ExitButtonClicked);

	return true;
}

void UMainMenuWidget::SingleplayButtonClicked()
{
	// TODO
	// 싱글플레이 모드로 들어간다 그러면 방 안만들고 어쩌고 저쩌고 안하고 바로 시작하기
}

void UMainMenuWidget::MultiplayButtonClicked()
{
	// TODO
	// 멀티플레이 모드로 들어간다 방 만들고 찾고 하는 기능 필요함
}

void UMainMenuWidget::OptionButtonClicked()
{
	// TODO
	// 여러가지 옵션을 설정함
}

void UMainMenuWidget::ExitButtonClicked()
{
	// TODO
	// 게임 끌게
	APlayerController* PlayerController = GetOwningPlayer();
	if(PlayerController)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}
