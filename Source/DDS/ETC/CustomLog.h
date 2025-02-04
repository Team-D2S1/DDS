// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

// [파일명::함수명 - 라인번호] : 내용
// 커스텀 로그 - 나중에 로그 지우기 쉽게, 어디서 실행됐는지 알기 편하게
#define MY_LOG(Category, Verbosity, Format, ...) \
UE_LOG(Category, Verbosity, TEXT("[%s - %d] : ") Format, ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__)
