// Created By Tong
// 玩家控制器

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharController.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API APlayerCharController : public APlayerController
{
	GENERATED_BODY()
public:
	APlayerCharController();
	UPROPERTY()
	UMaterialInstance* post;
	
	virtual void SetupInputComponent() override;
	UFUNCTION(BlueprintCallable)
	void MoveForward(float val);
	UFUNCTION(BlueprintCallable)
		void MoveRight(float val);
	UFUNCTION(BlueprintCallable)
		void LookUp(float val);
	UFUNCTION(BlueprintCallable)
		void LookRight(float val);
	UFUNCTION(BlueprintCallable)
		void DoJump();
	UFUNCTION(BlueprintCallable)
		void DoAttack();
	UFUNCTION(BlueprintCallable)
		void DoRoll();
	UFUNCTION(BlueprintCallable)
		void DoRun();
	UFUNCTION(BlueprintCallable)
		void DoWalk();
	UFUNCTION(BlueprintCallable)
		void DoChangeMeshSkin();
	UFUNCTION(BlueprintCallable)
		void DoChangeChestSkin();
	UFUNCTION(BlueprintCallable)
		void DoChangeShieldSkin();
	UFUNCTION(BlueprintCallable)
		void DoChangeSwordSkin();
	UFUNCTION(BlueprintCallable)
		void DoChangeAllSkin();
};
