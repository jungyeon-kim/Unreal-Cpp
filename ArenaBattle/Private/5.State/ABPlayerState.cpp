#include "ABPlayerState.h"

AABPlayerState::AABPlayerState()
{
	CharacterLevel = 1;
}

void AABPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("KJY"));
	CharacterLevel = 5;
}

int32 AABPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 AABPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}
