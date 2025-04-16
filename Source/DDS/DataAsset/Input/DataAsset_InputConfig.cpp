// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DataAsset_InputConfig.h"

#include "InputAction.h"

UInputAction* UDataAsset_InputConfig::FindInputActionByTag(const FGameplayTag& Tag) const
{
	for (const FDDSInputActionConfig& Config : NativeInputActions)
	{
		if (Config.InputTag.MatchesTag(Tag))
		{
			return Config.InputAction;
		}
	}
	return nullptr;
}

UInputAction* UDataAsset_InputConfig::FindUIActionByTag(const FGameplayTag& Tag) const
{
	for (const FDDSInputActionConfig& Config : UIInputActions)
	{
		if (Config.InputTag.MatchesTag(Tag))
		{
			return Config.InputAction;
		}
	}
	return nullptr;
}
