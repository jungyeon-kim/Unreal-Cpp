#include "BTTask_FindPatrolPos.h"
#include "ABAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	const auto& ControllingPawn{ OwnerComp.GetAIOwner()->GetPawn() };
	if (!ControllingPawn) return EBTNodeResult::Failed;

	const auto& NavSystem{ UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld()) };
	if (!NavSystem) return EBTNodeResult::Failed;

	FVector Origin{ OwnerComp.GetBlackboardComponent()->GetValueAsVector(AABAIController::HomePosKey) };
	FNavLocation NextPatrol{};

	if (NavSystem->GetRandomPointInNavigableRadius({ 0.0f, 0.0f, 0.0f }, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AABAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
