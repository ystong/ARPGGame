// Created By Tong


#include "BTTaskNode_ResetArrowBlackBoard.h"
#include "ArrowAIController.h"
#include "ArrowAIChar.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTTaskNode_ResetArrowBlackBoard::UBTTaskNode_ResetArrowBlackBoard()
{
	bCreateNodeInstance=true;
}

EBTNodeResult::Type UBTTaskNode_ResetArrowBlackBoard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AArrowAIController* AIC=Cast<AArrowAIController>(OwnerComp.GetAIOwner());
	AArrowAIChar* ArrowAI=Cast<AArrowAIChar>(AIC->GetPawn());
	ArrowAI->GetCharacterMovement()->MaxWalkSpeed=500.f;
	
	UBTFunctionLibrary::ClearBlackboardValue(GetNodeInstance(OwnerComp,NodeMemory),TargetPlayer);
	UBTFunctionLibrary::ClearBlackboardValue(GetNodeInstance(OwnerComp,NodeMemory),PlayerLocation);
	UBTFunctionLibrary::ClearBlackboardValue(GetNodeInstance(OwnerComp,NodeMemory),Destination);
	UBTFunctionLibrary::ClearBlackboardValue(GetNodeInstance(OwnerComp,NodeMemory),LeavePlayerLocation);
	
	return EBTNodeResult::Succeeded;
}
