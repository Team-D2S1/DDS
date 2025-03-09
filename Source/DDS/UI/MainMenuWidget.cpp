// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

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
