#include "ABItemBox.h"
#include "ABWeapon.h"
#include "ABCharacter.h"

AABItemBox::AABItemBox()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	WeaponItemClass = AABWeapon::StaticClass();

	RootComponent = Trigger;
	Box->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetBoxExtent({ 40.0f, 42.0f, 30.0f });
	Box->SetRelativeLocation({ 0.0f, -3.5f, -30.0f });

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX{ TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1") };
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN{ TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh") };
	if (SM_BOX.Succeeded()) Box->SetStaticMesh(SM_BOX.Object);
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;		// 해당 파티클을 자동으로 활성화하지 않는다.
	}

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));
}

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
}

void AABItemBox::BeginPlay()
{
	Super::BeginPlay();
}

void AABItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlapedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABLOG_S(Warning);

	const auto& ABCharacter{ Cast<AABCharacter>(OtherActor) };
	ABCHECK(ABCharacter && WeaponItemClass);

	if (ABCharacter->CanSetWeapon())
	{
		const auto& NewWeapon{ GetWorld()->SpawnActor<AABWeapon>(WeaponItemClass) };
		ABCharacter->SetWeapon(NewWeapon);
		Effect->Activate(true);
		Box->SetHiddenInGame(true, true);	// 게임플레이 화면에서만 숨긴다.
		SetActorEnableCollision(false);
	}
	else ABLOG(Warning, TEXT("%s can't equip weapon currently."), *ABCharacter->GetName());
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}