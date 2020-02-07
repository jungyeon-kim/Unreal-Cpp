#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
// �Ϲ� �α�
#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
// ��� �α� (���ڿ��� *�� �پ���մϴ�.)
#define ABLOG(Verbosity, Format, ...) UE_LOG(ArenaBattle, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
// ������ ���� �ƴѰ�� ���� �α׸� ���� �����մϴ�. ��ȯ ���� �ִ� �Լ������� �ι�°���ڷ� ��ȯ���� �ο��ؾ��մϴ�.
#define ABCHECK(Expr, ...) { if(!(Expr)) { ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }

UENUM()
enum class ECharacterState
{
	INIT,
	LOADING,
	READY,
	DEAD
};