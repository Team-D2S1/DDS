// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AnimInstances/Player/DDSPlayerLinkedAnimLayer.h"

#include "DDSPlayerAnimInstance.h"

UDDSPlayerAnimInstance* UDDSPlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UDDSPlayerAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
