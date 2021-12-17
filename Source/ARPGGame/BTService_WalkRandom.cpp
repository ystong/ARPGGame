// Created By Tong


#include "BTService_WalkRandom.h"
#include "SwordAIController.h"
#include "SwordAIChar.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BTFunctionLibrary.h"

UBTService_WalkRandom::UBTService_WalkRandom()
{
	bCreateNodeInstance=true;
}

void UBTService_WalkRandom::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//获取半径1000范围内可到达的坐标，并设置为黑板键
	ASwordAIController* AIC=Cast<ASwordAIController>(OwnerComp.GetAIOwner());
	FVector AILocation=Cast<ASwordAIChar>(AIC->GetPawn())->GetActorLocation();
	UNavigationSystemV1* NavMesh = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation RandomPt;
	NavMesh->GetRandomReachablePointInRadius(AILocation,1000.f,RandomPt);
	UBTFunctionLibrary::SetBlackboardValueAsVector(GetNodeInstance(OwnerComp,NodeMemory),Destination, RandomPt.Location);
}
