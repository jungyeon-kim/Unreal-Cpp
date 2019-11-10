#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn{ TryGetPawnOwner() };	// ���� ��ȿ���� �˻�
	if (IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		ACharacter* Character{ Cast<ACharacter>(Pawn) };	// IsFalling()�� Pawn���� ���� ������ ĳ����
		if (Character) IsOnAir = Character->GetMovementComponent()->IsFalling();
	}
}
