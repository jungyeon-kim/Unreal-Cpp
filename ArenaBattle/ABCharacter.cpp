#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "ABCharacterStatComponent.h"
#include "ABWeapon.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "ABCharacterWidget.h"

AABCharacter::AABCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));

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
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C"));
	if (SK_CHARM_GOLDEN.Succeeded()) GetMesh()->SetSkeletalMesh(SK_CHARM_GOLDEN.Object);
	if (WARRIOR_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize({ 150.0f, 50.0f });
	}

	SetControlMode(EControlMode::GTA);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));	// 콜리전 프리셋 변경

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;
	GetCharacterMovement()->JumpZVelocity = 450.0f;
	MaxCombo = 4;
	AttackLength = 200.0f;
	AttackRadius = 50.0f;
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(ABAnim);
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);	// 멀티캐스트 다이나믹 델리게이트
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
	CharacterStat->OnHPIsZero.AddLambda([&]()
	{
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	});
}

void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	//auto CurWeapon{ GetWorld()->SpawnActor<AABWeapon>() };
	//if (CurWeapon) CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	//	TEXT("hand_rSocket"));

	// UI 시스템은 플레이어 컨트롤러의 BeginPlay()에서 생성된다.
	// 따라서, 이 클래스의 PostInitializeComponents()은 위젯 생성에 효능이 없어 BeginPlay()에서 작업해준다.
	const auto& CharacterWidget{ Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject()) };
	if (CharacterWidget) CharacterWidget->BindCharacterStat(CharacterStat);
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

// OnMontageEnded 딜리게이트에 바인딩
void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ABCHECK(bIsAttacking);
	ABCHECK(CurrentCombo > 0);
	bIsAttacking = false;
	AttackEndComboState();
}

// 공격이 시작할 때 상태 세팅
void AABCharacter::AttackStartComboState()
{
	bCanNextCombo = true;
	bIsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

// 공격이 끝날 때 상태 세팅
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
		ECollisionChannel::ECC_GameTraceChannel2,	// Attack 트레이스 채널
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
		bUseControllerRotationYaw = false;								// 컨트롤러의 Yaw회전을 캐릭터에 적용
		GetCharacterMovement()->bOrientRotationToMovement = true;		// 움직이는 방향으로 회전
		GetCharacterMovement()->bUseControllerDesiredRotation = false;	// RotationRate에 따라 부드럽게 회전
		GetCharacterMovement()->RotationRate = { 0.0f, 720.0f, 0.0f };	// 초당 회전량
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
	}
}