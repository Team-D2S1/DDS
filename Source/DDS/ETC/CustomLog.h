// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

// [파일명::함수명 - 라인번호] : 내용
// 커스텀 로그 - 나중에 로그 지우기 쉽게, 어디서 실행됐는지 알기 편하게
#define MY_LOG(Category, Verbosity, Format, ...) \
UE_LOG(Category, Verbosity, TEXT("[%s - %d] : ") Format, ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__)


namespace Debug
{
	// 로그 타입(ELogVerbosity)과 메시지, 색
	static void Log(const FString& Message,const FColor& Color = FColor::MakeRandomColor())
	{
		MY_LOG(LogTemp, Type::Log, "%s", *Message);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, Message);
		}
	}

	static void LogError(const FString& Message,const FColor& Color = FColor::MakeRandomColor())
	{
		MY_LOG(LogTemp, Type::Error, "%s", *Message);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, Message);
		}
	}
}
