#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
private:
	GENERATED_BODY()
public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;
};
