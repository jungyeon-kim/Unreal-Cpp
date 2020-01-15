#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bIsAttacking;		// ���������� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bCanNextCombo;		// ���� �޺� ���డ�� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bIsComboInputOn;	// �޺� �Է� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;		// ���� �������� �޺�
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;			// �޺��� �ִ�ġ
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackLength;		// �������� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRadius;		// �������� ������
	UPROPERTY()
	class UABAnimInstance* ABAnim;
private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void ViewChange();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();
protected:
	enum class EControlMode
	{
		GTA, DIABLO, NPC
	};
protected:
	float ArmLengthTo{};
	float ArmLengthSpeed{};
	float ArmRotationSpeed{};
	FRotator ArmRotationTo{};
	EControlMode CurrentControlMode{};
	FVector DirectionToMove{};
protected:
	virtual void BeginPlay() override;
	void SetControlMode(EControlMode ControlMode);
public:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* HPBarWidget;
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	class UABCharacterStatComponent* CharacterStat;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class AABWeapon* CurrentWeapon;
public:
	AABCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewController) override;

	void Attack();
	FOnAttackEndDelegate OnAttackEnded{};

	bool CanSetWeapon();
	void SetWeapon(class AABWeapon* NewWeapon);
};
