// Created By Tong
// 行为树任务节点 重置大刀兵AI的黑板键

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ResetBlackBoardKey.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UBTTaskNode_ResetBlackBoardKey : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector TargetPlayer;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector PlayerLocation;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector Destination;
	
	UBTTaskNode_ResetBlackBoardKey();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
