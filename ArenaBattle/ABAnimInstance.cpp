#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn{ TryGetPawnOwner() };	// 폰이 유효한지 검사
	if (IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		ACharacter* Character{ Cast<ACharacter>(Pawn) };	// IsFalling()이 Pawn에는 없기 때문에 캐스팅
		if (Character) IsOnAir = Character->GetMovementComponent()->IsFalling();
	}
}
