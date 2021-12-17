// Created By Tong
// 行为树装饰器 判断是否需要冲刺


#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsSpurt.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API UBTDecorator_IsSpurt : public UBTDecorator
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FBlackboardKeySelector TargetPlayer;
	
public:
	UBTDecorator_IsSpurt();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
