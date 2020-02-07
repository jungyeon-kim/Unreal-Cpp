#include "ABGameMode.h"
#include "ABPlayerController.h"
#include "ABCharacter.h"
#include "ABPlayerState.h"

AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
	PlayerStateClass = AABPlayerState::StaticClass();
}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	const auto& ABPlayerState{ Cast<AABPlayerState>(NewPlayer->PlayerState) };
	ABCHECK(ABPlayerState);
	ABPlayerState->InitPlayerData();
}
