#include "ABCharacter.h"
#include "ABPlayerController.h"
#include "ABAIController.h"
#include "ABWeapon.h"
#include "ABCharacterStatComponent.h"
#include "ABAnimInstance.h"
#include "ABCharacterWidget.h"
#include "ABCharacterSetting.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"

AABCharacter::AABCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	RootComponent = GetCapsuleComponent();
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation({ 0.0f, 0.0f, -88.0f }, { 0.0f, -90.0f, 0.0f });
	SpringArm->SetRelativeRotation({ -15.0f, 0.0f, 0.0f });
	SpringArm->TargetArmLength = 400.0f;
	HPBarWidget->SetRelativeLocation({ 0.0f, 0.0f, 180.0f });
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARM_GOLDEN{ TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Golden.SK_CharM_Golden") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM{ TEXT("/Game/Book/Animation/WarriorAnimBlueprint.WarriorAnimBlueprint_C") };
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD{ TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C" });
	if (SK_CHARM_GOLDEN.Succeeded()) GetMesh()->SetSkeletalMesh(SK_CHARM_GOLDEN.Object);
	if (WARRIOR_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize({ 150.0f, 50.0f });
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));	// �ݸ��� ������ ����

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;
	GetCharacterMovement()->JumpZVelocity = 450.0f;
	MaxCombo = 4;
	AttackLength = 200.0f;
	AttackRadius = 50.0f;

	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	bCanBeDamaged = false;
	SetCharacterState(ECharacterState::LOADING);

	DeadTimer = 5.0f;
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(ABAnim);
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);	// ��Ƽĳ��Ʈ ���̳��� ��������Ʈ
	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);
	ABAnim->OnNextAttackCheck.AddLambda([&]()
	{
		bCanNextCombo = false;
		if (bIsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});
}

void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	//auto CurWeapon{ GetWorld()->SpawnActor<AABWeapon>() };
	//if (CurWeapon) CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	//	TEXT("hand_rSocket"));

	bIsPlayer = IsPlayerControlled();

	if (bIsPlayer)
	{
		ABPlayerController = Cast<AABPlayerController>(GetController());
	}
	else
	{
		ABAIController = Cast<AABAIController>(GetController());
	}

	SetCharacterState(ECharacterState::READY);

	// UI �ý����� �÷��̾� ��Ʈ�ѷ��� BeginPlay()���� �����ȴ�.
	// ����, �� Ŭ������ �����ڳ� PostInitializeComponents()������ ���� ������ ȿ���� ���� BeginPlay()���� �۾����ش�.
}

void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(
		SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		SpringArm->RelativeRotation = FMath::RInterpTo(
			SpringArm->RelativeRotation, ArmRotationTo, DeltaTime, ArmRotationSpeed);

		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	}
}

void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AABCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
}

float AABCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void AABCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

bool AABCharacter::CanSetWeapon()
{
	return !CurrentWeapon;
}

void AABCharacter::SetWeapon(AABWeapon* NewWeapon)
{
	ABCHECK(NewWeapon && !CurrentWeapon);

	if (NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
			TEXT("hand_rSocket"));
		CurrentWeapon = NewWeapon;
	}
}

ECharacterState AABCharacter::GetCharacterState() const
{
	return CurrentState;
}

void AABCharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		bCanBeDamaged = false;

		if (bIsPlayer) DisableInput(ABPlayerController);
		break;
	}
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		bCanBeDamaged = true;

		CharacterStat->OnHPIsZero.AddLambda([&]()
		{
			SetCharacterState(ECharacterState::DEAD);
		});

		const auto& CharacterWidget{ Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject()) };
		if (CharacterWidget) CharacterWidget->BindCharacterStat(CharacterStat);

		if (bIsPlayer)
		{
			SetControlMode(EControlMode::GTA);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(ABPlayerController);
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			ABAIController->RunAI();
		}
		break;
	}
	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		ABAnim->SetDeadAnim();
		bCanBeDamaged = false;

		if (bIsPlayer) DisableInput(ABPlayerController);
		else ABAIController->StopAI();

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
			[&]()
		{
			if (bIsPlayer) ABPlayerController->RestartLevel();
			else Destroy();
		}
		), DeadTimer, false);
		break;
	}
	}
}

void AABCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.X = NewAxisValue;
		break;
	}
}

void AABCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue;
		break;
	}
}

void AABCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
}

void AABCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;
	}
}

void AABCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->RelativeRotation);
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AABCharacter::Attack()
{
	if (bIsAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (bCanNextCombo) bIsComboInputOn = true;
	}
	else
	{
		ABCHECK(!CurrentCombo);
		AttackStartComboState();
		ABAnim->PlayAttackMontange();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		bIsAttacking = true;
	}
}

// OnMontageEnded ��������Ʈ�� ���ε�
void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(bIsAttacking);
	ABCHECK(CurrentCombo > 0);
	bIsAttacking = false;
	AttackEndComboState();
	OnAttackEnded.Broadcast();
}

// ������ ������ �� ���� ����
void AABCharacter::AttackStartComboState()
{
	bCanNextCombo = true;
	bIsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

// ������ ���� �� ���� ����
void AABCharacter::AttackEndComboState()
{
	bIsComboInputOn = false;
	bCanNextCombo = false;
	CurrentCombo = 0;
}

void AABCharacter::AttackCheck()
{
	FHitResult HitResult{};
	FCollisionQueryParams Params{ NAME_None, false, this };
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackLength,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,	// Attack Ʈ���̽� ä��
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector Trace{ GetActorForwardVector() * AttackLength };
	FVector Center{ GetActorLocation() + Trace * 0.5f };
	float HalfHeight{ AttackLength * 0.5f + AttackRadius };
	FQuat CapsuleRot{ FRotationMatrix::MakeFromZ(Trace).ToQuat() };
	FColor DrawColor{ bResult ? FColor::Green : FColor::Red };
	float DebugLifeTime{ 5.0f };

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
#endif

	if (bResult)
		if (HitResult.Actor.IsValid())
		{
			ABLOG(Warning, TEXT("Hit Actor Name: %s"), *HitResult.Actor->GetName());
			
			FDamageEvent DamageEvent{};
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
		}
}

void AABCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		//SpringArm->TargetArmLength = 450.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 450.0f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;								// ��Ʈ�ѷ��� Yawȸ���� ĳ���Ϳ� ����
		GetCharacterMovement()->bOrientRotationToMovement = true;		// �����̴� �������� ȸ��
		GetCharacterMovement()->bUseControllerDesiredRotation = false;	// RotationRate�� ���� �ε巴�� ȸ��
		GetCharacterMovement()->RotationRate = { 0.0f, 720.0f, 0.0f };	// �ʴ� ȸ����
		break;
	case EControlMode::DIABLO:
		//SpringArm->TargetArmLength = 800;
		//SpringArm->SetRelativeRotation({ -45.0f, 0.0f, 0.0f });
		ArmLengthTo = 800.0f;
		ArmRotationTo = { -45.0f, 0.0f, 0.0f };
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = { 0.0f, 720.0f, 0.0f };
		break;
	case EControlMode::NPC:

		SpringArm->bInheritYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = { 0.0f, 480.0f, 0.0f };
		break;
	}
}