#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

UCLASS()
class ARENABATTLE_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()
protected:
	UPROPERTY(Transient)
	int32 GameScore;
	UPROPERTY(Transient)
	int32 CharacterLevel;
public:
	AABPlayerState();

	void InitPlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;
};
