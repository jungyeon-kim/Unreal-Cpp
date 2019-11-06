#include "ABPawn.h"

AABPawn::AABPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AABPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AABPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABLOG_S(Warning);
}

void AABPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ABLOG_S(Warning);
}

void AABPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

