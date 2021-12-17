// Created By Tong
// 行为树装饰器 判断是否有玩家出现在警戒范围内

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_HavePlayer_ArrowAI.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UBTDecorator_HavePlayer_ArrowAI : public UBTDecorator
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector TargetPlayer;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector PlayerLocation;
	
	UBTDecorator_HavePlayer_ArrowAI();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};


