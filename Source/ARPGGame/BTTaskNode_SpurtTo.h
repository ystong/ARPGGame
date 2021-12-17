// Created By Tong
// 行为树任务节点 执行冲刺

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_SpurtTo.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UBTTaskNode_SpurtTo : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	struct FBlackboardKeySelector TargetPlayer;

public:
	UBTTaskNode_SpurtTo();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
