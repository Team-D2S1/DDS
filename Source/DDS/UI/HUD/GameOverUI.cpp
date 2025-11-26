// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "UI/HUD/GameOverUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ETC/CustomLog.h"


void UGameOverUI::ShowGameOver()
{
	MY_LOG(LogTemp, Error, TEXT("2"))
	
	SetVisibility(ESlateVisibility::Visible);

	if(GameOverAnim)
	{
		PlayAnimation(GameOverAnim);

		FTimerHandle AnimFinishHandle;
		GetWorld()->GetTimerManager().SetTimer(AnimFinishHandle, this,
			&ThisClass::OnFadeInFinished, 6.f, false);
	}
}

void UGameOverUI::HideGameOver()
{
	SetVisibility(ESlateVisibility::Hidden);

	if(GameOverBackground)
	{
		GameOverBackground->SetRenderOpacity(0.f);
	}

	if(GameOverText)
	{
		GameOverText->SetRenderOpacity(0.f);
	}
}

void UGameOverUI::NativeConstruct()
{
	Super::NativeConstruct();

	if(GameOverBackground)
	{
		GameOverBackground->SetRenderOpacity(0.f);
	}

	if(GameOverText)
	{
		GameOverText->SetRenderOpacity(0.f);
	}

	SetVisibility(ESlateVisibility::Hidden);
}



void UGameOverUI::OnFadeInFinished()
{
	MY_LOG(LogTemp, Error, TEXT("리스폰 ㅋㅋ"))
	HideGameOver();
}
