// Created By Tong
// 行为树任务节点 重置弓箭手AI的黑板键

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ResetArrowBlackBoard.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UBTTaskNode_ResetArrowBlackBoard : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector TargetPlayer;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector PlayerLocation;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector Destination;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector LeavePlayerLocation;
	
	UBTTaskNode_ResetArrowBlackBoard();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
