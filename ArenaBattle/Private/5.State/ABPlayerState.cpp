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

void AABPlayerState::AddGameScore()
{
	++GameScore;
	OnPlayerStateChanged.Broadcast();
}

int32 AABPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 AABPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}
