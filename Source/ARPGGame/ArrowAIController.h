// Created By Tong
// 弓箭手AI控制器

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ArrowAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API AArrowAIController : public AAIController
{
	GENERATED_BODY()
public:
	AArrowAIController();
	
	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	void StopBT();
	void RunBT();
};
