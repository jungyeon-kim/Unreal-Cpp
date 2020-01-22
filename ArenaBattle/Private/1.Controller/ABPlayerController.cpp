#include "ABPlayerController.h"

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//ABLOG_S(Warning);
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AABPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//ABLOG_S(Warning);
}
