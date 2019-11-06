#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Pawn.h"
#include "ABPawn.generated.h"

UCLASS()
class ARENABATTLE_API AABPawn : public APawn
{
	GENERATED_BODY()
public:
	AABPawn();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController);
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
