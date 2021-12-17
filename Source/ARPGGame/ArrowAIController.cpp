// Created By Tong


#include "ArrowAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AArrowAIController::AArrowAIController()
{
	PrimaryActorTick.bCanEverTick=true;
}

void AArrowAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//加载行为树并运行
	BehaviorTree=LoadObject<UBehaviorTree>(nullptr,TEXT("BehaviorTree'/Game/AI/Arrow/ArrowAIBT.ArrowAIBT'"));
	RunBehaviorTree(BehaviorTree);
}

void AArrowAIController::OnUnPossess()
{
	Super::OnUnPossess();
	Cast<UBehaviorTreeComponent>(BrainComponent)->StopTree();
}


//停止当前行为树
void AArrowAIController::StopBT()
{
	Cast<UBehaviorTreeComponent>(BrainComponent)->StopTree();
}

//重启行为树
void AArrowAIController::RunBT()
{
	RunBehaviorTree(BehaviorTree);
}
