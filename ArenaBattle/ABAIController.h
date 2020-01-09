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
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
public:
	AABAIController();
	virtual void OnPossess(APawn* InPawn) override;
};
