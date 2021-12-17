// Created By Tong


#include "BTTaskNode_ResetBlackBoardKey.h"
#include "SwordAIChar.h"
#include "SwordAIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTaskNode_ResetBlackBoardKey::UBTTaskNode_ResetBlackBoardKey()
{
	bCreateNodeInstance=true;
}

EBTNodeResult::Type UBTTaskNode_ResetBlackBoardKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//重置黑板键和移动速度
	ASwordAIController* AIC=Cast<ASwordAIController>(OwnerComp.GetAIOwner());
	ASwordAIChar* SwordAI=Cast<ASwordAIChar>(AIC->GetPawn());
	SwordAI->GetCharacterMovement()->MaxWalkSpeed=500.f;
	
	UBTFunctionLibrary::ClearBlackboardValue(GetNodeInstance(OwnerComp,NodeMemory),TargetPlayer);
	UBTFunctionLibrary::ClearBlackboardValue(GetNodeInstance(OwnerComp,NodeMemory),PlayerLocation);
	UBTFunctionLibrary::ClearBlackboardValue(GetNodeInstance(OwnerComp,NodeMemory),Destination);
	return EBTNodeResult::Succeeded;
}
