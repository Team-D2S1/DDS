// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "UI/HUD/GameOverUI.h"

#include "Character/Player/PlayerBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ETC/CustomLog.h"
#include "PlayerController/InGamePlayerController.h"


void UGameOverUI::ShowGameOver()
{	
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
	// 리스폰 콜백
	if(GetOwningPlayer())
	{
		if(AInGamePlayerController* PC = Cast<AInGamePlayerController>(GetOwningPlayer()))
		{
			PC->OnPlayerDeathEnd();
		}
	}
	
	HideGameOver();
}
