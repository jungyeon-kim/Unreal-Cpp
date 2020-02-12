#include "ABGameState.h"
#include "..\..\Public\5.State\ABGameState.h"

AABGameState::AABGameState()
{
}

void AABGameState::AddGameScore()
{
	++TotalGameScore;
}

int32 AABGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}