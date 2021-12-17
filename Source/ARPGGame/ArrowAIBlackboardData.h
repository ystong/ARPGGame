// Created By Tong
// 弓箭手AI黑板数据

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardData.h"
#include "ArrowAIBlackboardData.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UArrowAIBlackboardData : public UBlackboardData
{
	GENERATED_BODY()
	public:
		FBlackboardEntry Destination;
		FBlackboardEntry PlayerLocation;
		FBlackboardEntry TargetPlayer;
		FBlackboardEntry LeavePlayerLocation;
	
		virtual void PostLoad() override;
		
};
