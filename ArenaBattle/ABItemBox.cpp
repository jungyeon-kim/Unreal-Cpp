#include "ABItemBox.h"
#include "ABWeapon.h"

AABItemBox::AABItemBox()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	WeaponItemClass = AABWeapon::StaticClass();

	RootComponent = Trigger;
	Box->SetupAttachment(Trigger);

	Trigger->SetBoxExtent({ 40.0f, 42.0f, 30.0f });
	Box->SetRelativeLocation({ 0.0f, -3.5f, -30.0f });

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_BOX.Succeeded()) Box->SetStaticMesh(SM_BOX.Object);

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));
}

void AABItemBox::BeginPlay()
{
	Super::BeginPlay();
}

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}

void AABItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlapedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABLOG_S(Warning);
}