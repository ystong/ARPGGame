// Created By Tong
// 行为树装饰器，判断是否距离玩家太近

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CloseToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UBTDecorator_CloseToPlayer : public UBTDecorator
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector LeavePlayerLocation;
	
	UBTDecorator_CloseToPlayer();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

