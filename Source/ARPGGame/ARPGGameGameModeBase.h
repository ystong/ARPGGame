// Copyright Epic Games, Inc. All Rights Reserved.
// Created By Tong
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARPGGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ARPGGAME_API AARPGGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Lives;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Score;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int EnemyNum;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int KilledEmemy;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int HitCount;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int HitCountSave;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int HitCountFrame;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsGameOver;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class TSubclassOf<UUserWidget> ScreenWidget;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class TSubclassOf<UUserWidget> GameOverWidget;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class TSubclassOf<UUserWidget> HitCountWidget;
	UPROPERTY()
	class UProgressBar* HPBarProgress;
	UPROPERTY()
	class UTextBlock* ScoreText;
	UPROPERTY()
	class UTextBlock* EnemyCountText;
	UPROPERTY()
	class UTextBlock* FinalScoreText;
	UPROPERTY()
	class UTextBlock* HitCountText;
	UPROPERTY()
	class UTextBlock* LivesText;
	UPROPERTY()
	class UUserWidget* CurrentWidget;
	UPROPERTY()
	class UUserWidget* HitCountUserWidget;

	
	AARPGGameGameModeBase();
	UFUNCTION(BlueprintCallable)
	void ReLive();
	UFUNCTION(BlueprintCallable)
	void ScoreAdd(int val);
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	UFUNCTION(BlueprintCallable)
	void GameOver();
	UFUNCTION(BlueprintCallable)
	void HitCountReset();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
};
