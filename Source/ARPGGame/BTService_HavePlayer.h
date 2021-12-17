// Created By Tong
// 行为树装饰节点 不断检测周围是否有玩家

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_HavePlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UBTService_HavePlayer : public UBTService
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	struct FBlackboardKeySelector TargetPlayer;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector PlayerLocation;

public:
	UBTService_HavePlayer();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
