#pragma once

#include "ArenaBattle.h"
#include "GameFramework/GameStateBase.h"
#include "ABGameState.generated.h"

UCLASS()
class ARENABATTLE_API AABGameState : public AGameStateBase
{
	GENERATED_BODY()
private:
	UPROPERTY(Transient)
	int32 TotalGameScore;
public:
	void AddGameScore();
	int32 GetTotalGameScore() const;
public:
	AABGameState();
};
