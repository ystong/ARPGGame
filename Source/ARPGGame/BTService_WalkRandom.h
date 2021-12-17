// Created By Tong
// 行为树服务节点 不断获取周围可到达的坐标

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_WalkRandom.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UBTService_WalkRandom : public UBTService
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	struct FBlackboardKeySelector Destination;
	
	UBTService_WalkRandom();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
