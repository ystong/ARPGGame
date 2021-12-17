// Created By Tong


#include "BTTaskNode_SpurtTo.h"

#include "BehaviorTree/BTFunctionLibrary.h"
#include "SwordAIController.h"
#include "SwordAIChar.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTTaskNode_SpurtTo::UBTTaskNode_SpurtTo()
{
	bCreateNodeInstance=true;
}

EBTNodeResult::Type UBTTaskNode_SpurtTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//将大刀兵AI的行走速度设置为5000，并向玩家类移动
	static AActor* Goal=UBTFunctionLibrary::GetBlackboardValueAsActor(GetNodeInstance(OwnerComp,NodeMemory),TargetPlayer);
	ASwordAIController* AIC=Cast<ASwordAIController>(OwnerComp.GetAIOwner());
	
	ASwordAIChar* SwordAI=Cast<ASwordAIChar>(AIC->GetPawn());
	FVector AILocation=SwordAI->GetActorLocation();
	
	SwordAI->GetCharacterMovement()->MaxWalkSpeed=5000.f;
	
	AIC->MoveToActor(Goal,100.f);
	
	return EBTNodeResult::Succeeded;
	
}
