// Created By Tong


#include "SwordAIController.h"
#include "SwordAIChar.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASwordAIController::ASwordAIController()
{
	PrimaryActorTick.bCanEverTick=true;
}

void ASwordAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	BehaviorTree=LoadObject<UBehaviorTree>(nullptr,TEXT("BehaviorTree'/Game/AI/Sword/SwordAIBT.SwordAIBT'"));
	if(BehaviorTree!=nullptr)
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void ASwordAIController::OnUnPossess()
{
	Super::OnUnPossess();
	Cast<UBehaviorTreeComponent>(BrainComponent)->StopTree();
	
}

void ASwordAIController::StopBT()
{
	Cast<UBehaviorTreeComponent>(BrainComponent)->StopTree();
}

void ASwordAIController::RunBT()
{
	if(BehaviorTree!=nullptr)
	{
		RunBehaviorTree(BehaviorTree);
	}
}
