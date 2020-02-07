#include "ABPlayerController.h"
#include "ABHUDWidget.h"
#include "ABPlayerState.h"

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if(UI_HUD.Succeeded()) HUDWidgetClass = UI_HUD.Class;
}

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<UABHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();

	const auto& ABPlayerState{ Cast<AABPlayerState>(PlayerState) };
	ABCHECK(ABPlayerState);
	HUDWidget->BindPlayerState(ABPlayerState);
	ABPlayerState->OnPlayerStateChanged.Broadcast();
}

void AABPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//ABLOG_S(Warning);
}

UABHUDWidget* AABPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}
