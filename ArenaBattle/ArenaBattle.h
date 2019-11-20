#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
// 일반 로그
#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
// 출력 로그 (문자열은 *이 붙어야합니다.)
#define ABLOG(Verbosity, Format, ...) UE_LOG(ArenaBattle, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
// 조건이 참이 아닌경우 붉은 로그를 띄우고 리턴합니다.
#define ABCHECK(Expr, ...) { if(!(Expr)) { ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }
