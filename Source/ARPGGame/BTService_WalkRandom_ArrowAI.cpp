// Created By Tong


#include "BTService_WalkRandom_ArrowAI.h"
#include "ArrowAIController.h"
#include "ArrowAIChar.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BTFunctionLibrary.h"

UBTService_WalkRandom_ArrowAI::UBTService_WalkRandom_ArrowAI()
{
	bCreateNodeInstance=true;
}

void UBTService_WalkRandom_ArrowAI::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//获取半径1000范围内可到达的坐标，并设置为黑板键
	AArrowAIController* AIC=Cast<AArrowAIController>(OwnerComp.GetAIOwner());
	FVector AILocation=Cast<AArrowAIChar>(AIC->GetPawn())->GetActorLocation();
	UNavigationSystemV1* NavMesh = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation RandomPt;
	NavMesh->GetRandomReachablePointInRadius(AILocation,1000.f,RandomPt);
	UBTFunctionLibrary::SetBlackboardValueAsVector(GetNodeInstance(OwnerComp,NodeMemory),Destination, RandomPt.Location);
}
