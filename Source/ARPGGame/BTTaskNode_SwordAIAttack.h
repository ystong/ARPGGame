// Created By Tong
// 行为树任务节点 大刀兵AI攻击

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_SwordAIAttack.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UBTTaskNode_SwordAIAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_SwordAIAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};


