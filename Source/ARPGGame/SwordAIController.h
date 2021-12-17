// Created By Tong
// 大刀兵AI控制器

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SwordAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API ASwordAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASwordAIController();
	
	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	void StopBT();
	void RunBT();
};
