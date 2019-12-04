#pragma once

#include "ArenaBattle.h"
#include "AIController.h"
#include "ABAIController.generated.h"

UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UBlackboardData* BBAsset;
	UPROPERTY()
	class UBehaviorTree* BTAsset;
public:
	FName HomePosKey{};
	FName PatrolPosKey{};
public:
	AABAIController();
	virtual void OnPossess(APawn* InPawn) override;
};
