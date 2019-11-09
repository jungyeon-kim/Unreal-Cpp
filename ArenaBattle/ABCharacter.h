// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void ViewChange();
protected:
	enum class EControlMode
	{
		GTA, DIABLO
	};

	float ArmLengthTo{};
	float ArmLengthSpeed{};
	float ArmRotationSpeed{};
	FRotator ArmRotationTo{};
	EControlMode CurrentControlMode{};
	FVector DirectionToMove{};

	virtual void BeginPlay() override;
	void SetControlMode(EControlMode ControlMode);
public:
	AABCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
};
