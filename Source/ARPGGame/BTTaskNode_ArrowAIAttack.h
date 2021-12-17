// Created By Tong
// 行为树任务节点 弓箭手AI攻击

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ArrowAIAttack.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UBTTaskNode_ArrowAIAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_ArrowAIAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

