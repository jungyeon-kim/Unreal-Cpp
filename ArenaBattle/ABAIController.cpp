#include "ABAIController.h"
//#include "NavigationSystem.h"
//#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


AABAIController::AABAIController()
{
	HomePosKey = TEXT("HomePos");
	PatrolPosKey = TEXT("PatrolPos");

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_Object{ TEXT("/Game/Book/AI/BB_ABCharacter.BB_ABCharacter") };
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_Object{ TEXT("/Game/Book/AI/BT_ABCharacter.BT_ABCharacter") };
	
	if (BB_Object.Succeeded()) BBAsset = BB_Object.Object;
	if (BT_Object.Succeeded()) BTAsset = BT_Object.Object;
}

void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ABCHECK(BTAsset && BBAsset);
	UseBlackboard(BBAsset, Blackboard);
	Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
	RunBehaviorTree(BTAsset);
}