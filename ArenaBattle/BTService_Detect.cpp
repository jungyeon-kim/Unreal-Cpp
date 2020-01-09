#include "BTService_Detect.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto& ControllingPawn{ OwnerComp.GetAIOwner()->GetPawn() };
	if (!ControllingPawn) return;

	const auto& World{ ControllingPawn->GetWorld() };
	FVector Center{ ControllingPawn->GetActorLocation() };
	float DetectRadius{ 300.0f };
	if (!World) return;

	TArray<FOverlapResult> OverlapResults{};
	FCollisionQueryParams CollisionQueryParam{ NAME_None, false, ControllingPawn };
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,	// Attack Ã¤³Î
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
		for (const auto& OverlapResult : OverlapResults)
		{
			const auto& ABCharacter{ Cast<AABCharacter>(OverlapResult.GetActor()) };

			if (ABCharacter)
			{
				if (ABCharacter->GetController()->IsPlayerController())
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, ABCharacter);
					DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
					return;
				}
				else
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, nullptr);
			}
		}
	else
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, nullptr);

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}