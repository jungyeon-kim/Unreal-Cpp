#include "ABGameMode.h"
#include "ABPlayerController.h"
#include "ABCharacter.h"
#include "ABPlayerState.h"
#include "ABGameState.h"

AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
	PlayerStateClass = AABPlayerState::StaticClass();
	GameStateClass = AABGameState::StaticClass();
}

void AABGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABGameState = Cast<AABGameState>(GameState);
}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	const auto& ABPlayerState{ Cast<AABPlayerState>(NewPlayer->PlayerState) };
	ABCHECK(ABPlayerState);
	ABPlayerState->InitPlayerData();
}

void AABGameMode::AddScore(AABPlayerController* Player)
{
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto& ABPlayerController{ Cast<AABPlayerController>(It->Get()) };

		if (ABPlayerController && Player == ABPlayerController)
		{
			ABPlayerController->AddGameScore();
			break;
		}
	}

	ABGameState->AddGameScore();
}
