// Created By Tong


#include "BTTaskNode_SwordAIAttack.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "SwordAIController.h"
#include "SwordAIChar.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTaskNode_SwordAIAttack::UBTTaskNode_SwordAIAttack()
{
	bCreateNodeInstance=true;
}

EBTNodeResult::Type UBTTaskNode_SwordAIAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//如果当前有正在播放的动画，就不攻击
	ASwordAIController* AIC=Cast<ASwordAIController>(OwnerComp.GetAIOwner());
	ASwordAIChar* SwordAI=Cast<ASwordAIChar>(AIC->GetPawn());

	if(IsValid(SwordAI->GetCurrentMontage()))
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		SwordAI->DoAttack();
		return EBTNodeResult::Succeeded;
	}
}