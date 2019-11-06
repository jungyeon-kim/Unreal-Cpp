#include "ABPlayerController.h"

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABLOG_S(Warning);
}

void AABPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ABLOG_S(Warning);
}
