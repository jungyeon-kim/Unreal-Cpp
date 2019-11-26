#include "ABCharacterStatComponent.h"
#include "ABGameInstance.h"

UABCharacterStatComponent::UABCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;		// InitializeComponent()가 호출되려면 필요

	Level = 1;
}

// PostInitializeComponents와 같다.
void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetNewLevel(Level);
}

void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UABCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UABCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	const auto& ABGameInstance{ Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())) };

	if (ABGameInstance) CurrentStatData = ABGameInstance->GetABCharacterData(NewLevel);
	if (CurrentStatData)
	{
		Level = NewLevel;
		CurrentHP = CurrentStatData->MaxHP;
	}
	else ABLOG(Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
}

void UABCharacterStatComponent::SetDamage(float NewDamage)
{
	ABCHECK(CurrentStatData);

	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);
	if (CurrentHP <= 0.0f) OnHPIsZero.Broadcast();
}

float UABCharacterStatComponent::GetAttack()
{
	ABCHECK(CurrentStatData, 0.0f);

	return CurrentStatData->Attack;
}
