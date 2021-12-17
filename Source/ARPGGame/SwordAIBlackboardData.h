// Created By Tong
// 大刀兵黑板数据

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardData.h"
#include "SwordAIBlackboardData.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API USwordAIBlackboardData : public UBlackboardData
{
	GENERATED_BODY()
public:
	FBlackboardEntry Destination;
	FBlackboardEntry PlayerLocation;
	FBlackboardEntry TargetPlayer;

	virtual void PostLoad() override;
};
