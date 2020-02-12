#include "ABSection.h"
#include "ABGameMode.h"
#include "ABPlayerController.h"
#include "ABCharacter.h"
#include "ABItemBox.h"

AABSection::AABSection()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	RootComponent = Mesh;

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABSection::OnTriggerBeginOverlap);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE{ TEXT("/Game/Book/StaticMesh/SM_SQUARE.SM_SQUARE") };
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE{ TEXT("/Game/Book/StaticMesh/SM_GATE.SM_GATE") };
	static FName GateSockets[]{ {TEXT("+XGate")}, {TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate")} };
	if (SM_SQUARE.Succeeded()) Mesh->SetStaticMesh(SM_SQUARE.Object);
	for (const FName& GateSocket : GateSockets)
	{
		ABCHECK(Mesh->DoesSocketExist(GateSocket));
		UStaticMeshComponent* NewGate{ CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString()) };
		NewGate->SetStaticMesh(SM_GATE.Object);
		NewGate->SetupAttachment(RootComponent, GateSocket);
		NewGate->SetRelativeLocation({ 0.0f, -80.5f, 0.0f });
		GateMeshs.Add(NewGate);

		UBoxComponent* NewGateTrigger{ 
			CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger"))) };
		NewGateTrigger->SetBoxExtent({ 100.0f, 100.0f, 300.0f });
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		NewGateTrigger->SetRelativeLocation({ 70.0f, 0.0f, 250.0f });
		NewGateTrigger->SetCollisionProfileName(TEXT("ABTrigger"));
		GateTriggers.Add(NewGateTrigger);

		NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABSection::OnGateTriggerBeginOverlap);
		NewGateTrigger->ComponentTags.Add(GateSocket);
	}
	Trigger->SetBoxExtent({ 775.0f, 775.0f, 300.0f });
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation({ 0.0f, 0.0f, 250.0f });
	Trigger->SetCollisionProfileName(TEXT("ABTrigger"));

	EnemySpawnTime = 2.0f;
	ItemBoxSpawnTime = 5.0f;
}

void AABSection::BeginPlay()
{
	Super::BeginPlay();

	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

void AABSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABSection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

void AABSection::SetState(ESectionState NewState)
{
	switch (NewState)
	{
	case ESectionState::READY:
		Trigger->SetCollisionProfileName(TEXT("ABTrigger"));
		for (UBoxComponent* GateTrigger : GateTriggers)
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		OpenGates(true);
		break;
	case ESectionState::BATTLE:
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		OpenGates(false);

		GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimerHandle,
			FTimerDelegate::CreateUObject(this, &AABSection::OnNPCSpawn), EnemySpawnTime, false);
		GetWorld()->GetTimerManager().SetTimer(SpawnItemBoxTimerHandle,
			FTimerDelegate::CreateLambda([&]()
		{
			FVector2D RandXY{ FMath::RandPointInCircle(600.0f) };
			GetWorld()->SpawnActor<AABItemBox>(GetActorLocation() + FVector(RandXY, 30.0f), FRotator::ZeroRotator);
		}),
				ItemBoxSpawnTime, false);
		break;
	case ESectionState::COMPLETE:
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
			GateTrigger->SetCollisionProfileName(TEXT("ABTrigger"));
		OpenGates(true);
		break;
	}

	CurrentState = NewState;
}

void AABSection::OpenGates(bool bOpen)
{
	for (UStaticMeshComponent* Gate : GateMeshs)
	{
		if(bOpen) Gate->SetRelativeRotation({0.0f, -90.0f, 0.0f});
		else Gate->SetRelativeRotation({ 0.0f, 0.0f, 0.0f });
	}
}

void AABSection::OnNPCSpawn()
{
	const auto& KeyNPC{ GetWorld()->
		SpawnActor<AABCharacter>(GetActorLocation() + FVector::UpVector * 88.0f, FRotator::ZeroRotator) };

	if (KeyNPC) KeyNPC->OnDestroyed.AddDynamic(this, &AABSection::OnKeyNPCDestroyed);
}

void AABSection::OnKeyNPCDestroyed(AActor* DestroyedAcrtor)
{
	const auto& ABCharacter{ Cast<AABCharacter>(DestroyedAcrtor) };
	ABCHECK(ABCharacter);

	const auto& ABPlayerController{ Cast<AABPlayerController>(ABCharacter->LastHitBy) };
	ABCHECK(ABPlayerController);

	const auto& ABGameMode{ Cast<AABGameMode>(GetWorld()->GetAuthGameMode()) };
	if (ABGameMode) ABGameMode->AddScore(ABPlayerController);

	SetState(ESectionState::COMPLETE);
}

void AABSection::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurrentState == ESectionState::READY) SetState(ESectionState::BATTLE);
}

void AABSection::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FName ComponentTag{ OverlappedComponent->ComponentTags[0] };
	FName SocketName{ *ComponentTag.ToString().Left(2) };
	if (!Mesh->DoesSocketExist(SocketName)) return;

	FVector NewLocation{ Mesh->GetSocketLocation(SocketName) };

	TArray<FOverlapResult> OverlapResult{};
	FCollisionQueryParams CollisionQueryParam{ NAME_None, false, this };
	FCollisionObjectQueryParams ObjectQueryParam{ FCollisionObjectQueryParams::InitType::AllObjects };
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResult,
		NewLocation,
		FQuat::Identity,
		ObjectQueryParam,
		FCollisionShape::MakeSphere(775.0f),
		CollisionQueryParam
	);

	if (!bResult) const auto& NewSection{ GetWorld()->SpawnActor<AABSection>(NewLocation, {0.0f,0.0f,0.0f}) };
	else ABLOG(Warning, TEXT("New Section area is not empty."));
}
