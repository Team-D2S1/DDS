// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

// [파일명::함수명 - 라인번호] : 내용
// 커스텀 로그 - 나중에 로그 지우기 쉽게, 어디서 실행됐는지 알기 편하게
#define MY_LOG(Category, Verbosity, Format, ...) \
UE_LOG(Category, Verbosity, TEXT("[%s - %d] : ") Format, ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__)


// 색 랜덤 로그
#define DEBUG_LOG_DISPLAY(Format, ...) \
Debug::Log(FString::Printf(TEXT("[%s - %d] : ") Format, ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__))

// 색 지정 가능한 로그
#define DEBUG_CLOG_DISPLAY(Color, Format, ...) \
Debug::Log(FString::Printf(TEXT("[%s - %d] : ") Format, ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__),Color)


// 색 랜덤 로그(서버여부 출력)
#define DEBUG_LOG_DISPLAY_NET(IS_SERVER,Format, ...) \
Debug::LogWithNetInfo(FString::Printf(TEXT("[%s - %d] : ") Format, ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__), IS_SERVER)
// 색 지정 로그 (서버여부 출력)
#define DEBUG_CLOG_DISPLAY_NET(Color,IS_SERVER,Format, ...) \
Debug::LogWithNetInfo(FString::Printf(TEXT("[%s - %d] : ") Format, ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__), IS_SERVER,Color)


namespace Debug
{
	// 로그 타입(ELogVerbosity)과 메시지, 색
	static void Log(const FString& Message,const FColor& Color = FColor::MakeRandomColor())
	{
		UE_LOG(LogTemp, Type::Log, TEXT("%s"), *Message);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, Message);
		}
	}

	static void LogWithNetInfo(const FString& Message, bool isServer ,const FColor& Color = FColor::MakeRandomColor())
	{
		FString ToPrint = FString::Printf(TEXT("[%s] %s "), isServer ? TEXT("Server") : TEXT("Client"), *Message);
		Log(ToPrint, Color);
	}

	static void LogError(const FString& Message,const FColor& Color = FColor::MakeRandomColor())
	{
		UE_LOG(LogTemp, Type::Error,TEXT("%s"), *Message);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, Message);
		}
	}

	static void LogErrorWithNetInfo(const FString& Message, bool isServer, const FColor& Color = FColor::MakeRandomColor())
	{
		FString ToPrint = FString::Printf(TEXT("[%s] %s "), isServer ? TEXT("Server") : TEXT("Client"), *Message);
		LogError(ToPrint, Color);
	}
}
