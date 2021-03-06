#include "ABPawn.h"

AABPawn::AABPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);
	Mesh->SetRelativeLocationAndRotation({ 0.0f, 0.0f, -88.0f }, { 0.0f, -90.0f, 0.0f });
	SpringArm->SetRelativeRotation({ -15.0f, 0.0f, 0.0f });
	SpringArm->TargetArmLength = 400.0f;

	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARM_GOLDEN{ TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Golden.SK_CharM_Golden") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM{ TEXT("/Game/Book/Animation/WarriorAnimBlueprint.WarriorAnimBlueprint_C") };
	if (SK_CHARM_GOLDEN.Succeeded()) Mesh->SetSkeletalMesh(SK_CHARM_GOLDEN.Object);
	if (WARRIOR_ANIM.Succeeded()) Mesh->SetAnimInstanceClass(WARRIOR_ANIM.Class);
}

void AABPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//ABLOG_S(Warning);
}

void AABPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AABPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//ABLOG_S(Warning);
}

void AABPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABPawn::LeftRight);
}

void AABPawn::UpDown(float NewAxisValue)
{
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void AABPawn::LeftRight(float NewAxisValue)
{
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}