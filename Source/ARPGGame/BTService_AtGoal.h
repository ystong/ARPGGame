// Created By Tong
//行为树服务节点 检测是否撞到玩家

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AtGoal.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UBTService_AtGoal : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_AtGoal();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable)
void DoFlyCallBack();
};
