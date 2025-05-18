// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ClientSocket.generated.h"

/**
 * 
 */
struct FSocketReceivedData
{
	bool bExist;
	TArray<FString> ReceivedData;

	FSocketReceivedData() : bExist(false) {}
	explicit FSocketReceivedData(const TArray<FString>& InReceiveData) : bExist(true), ReceivedData(InReceiveData) {}
};

UCLASS()
class DDS_API UClientSocket : public UObject
{
	GENERATED_BODY()

public:
	inline static FString ServerIP = "221.145.125.146";
	// 소켓 생성 후 통신작업 수행
	FSocketReceivedData CreateSocket(const FString RequestMessage, const FString& ExtraInfo, bool bIsLocal);
	
	// 스트링을 받아와 | 기준으로 나눈다
	TArray<FString> StringTokenizer(FString String);
};