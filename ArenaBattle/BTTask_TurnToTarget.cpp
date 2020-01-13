#include "BTTask_TurnToTarget.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result{ Super::ExecuteTask(OwnerComp, NodeMemory) };

	const auto& ABCharacter{ Cast<AABCharacter>(OwnerComp.GetAIOwner()->GetPawn()) };
	if (!ABCharacter) return EBTNodeResult::Failed;

	const auto& Target{ Cast<AABCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AABAIController::TargetKey)) };
	if (!Target) return EBTNodeResult::Failed;

	FVector ForwardVector{ Target->GetActorLocation() - ABCharacter->GetActorLocation() };
	ForwardVector.Z = 0.0f;
	FRotator TargetRot{ FRotationMatrix::MakeFromX(ForwardVector).Rotator() };
	ABCharacter->SetActorRotation(
		FMath::RInterpTo(ABCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 4.0f));
	
	return EBTNodeResult::Succeeded;
}
