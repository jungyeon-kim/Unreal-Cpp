#include "ABGameInstance.h"

UABGameInstance::UABGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER{ TEXT("/Game/Book/GameData/ABCharacterData.ABCharacterData") };
	if (DT_ABCHARACTER.Succeeded()) ABCharacterTable = DT_ABCHARACTER.Object;
}

void UABGameInstance::Init()
{
	Super::Init();
}

FABCharacterData* UABGameInstance::GetABCharacterData(int32 Level)
{
	return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Level), TEXT(""));
}
