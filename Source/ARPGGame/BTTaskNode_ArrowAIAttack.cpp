// Created By Tong


#include "BTTaskNode_ArrowAIAttack.h"
#include "ArrowAIController.h"
#include "ArrowAIChar.h"

UBTTaskNode_ArrowAIAttack::UBTTaskNode_ArrowAIAttack()
{
	bCreateNodeInstance=true;
}

EBTNodeResult::Type UBTTaskNode_ArrowAIAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//如果当前有正在播放的动画，就不攻击
	AArrowAIController* AIC=Cast<AArrowAIController>(OwnerComp.GetAIOwner());
	AArrowAIChar* ArrowAI=Cast<AArrowAIChar>(AIC->GetPawn());

	if(IsValid(ArrowAI->GetCurrentMontage()))
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		ArrowAI->DoAttack();
		return EBTNodeResult::Succeeded;
	}
}