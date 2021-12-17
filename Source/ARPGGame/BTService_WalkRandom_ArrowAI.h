// Created By Tong
// 行为树服务节点 不断获取周围可到达的坐标

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_WalkRandom_ArrowAI.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UBTService_WalkRandom_ArrowAI : public UBTService
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector Destination;
	
	UBTService_WalkRandom_ArrowAI();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
