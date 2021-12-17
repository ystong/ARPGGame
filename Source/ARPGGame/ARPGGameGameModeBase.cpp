// Copyright Epic Games, Inc. All Rights Reserved.
// Created By Tong


#include "ARPGGameGameModeBase.h"
#include "PlayerChar.h"
#include "PlayerCharController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "SwordAIChar.h"
#include "ArrowAIChar.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"

AARPGGameGameModeBase::AARPGGameGameModeBase()
{
	
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	//游戏相关属性初始化
	Lives=3;
	Score=0;
	EnemyNum=0;
	KilledEmemy=0;
	HitCount=0;
	HitCountFrame=0;
	IsGameOver=false;
	
	DefaultPawnClass=APlayerChar::StaticClass();
	PlayerControllerClass=APlayerCharController::StaticClass();
	
	ScreenWidget=LoadClass<UUserWidget>(NULL,TEXT("Blueprint'/Game/UI/GamePlayScreen.GamePlayScreen_C'"));
	GameOverWidget=LoadClass<UUserWidget>(NULL,TEXT("Blueprint'/Game/UI/RestartScreen.RestartScreen_C'"));
	HitCountWidget=LoadClass<UUserWidget>(NULL,TEXT("Blueprint'/Game/UI/ComboWidget.ComboWidget_C'"));
}

//复活
void AARPGGameGameModeBase::ReLive()
{
	if(Lives-->1)
	{
		FTransform StartTransform=FindPlayerStart(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetController())->GetActorTransform();
		APlayerChar* NewPlayer=GetWorld()->SpawnActor<APlayerChar>(APlayerChar::StaticClass(),StartTransform);
		UGameplayStatics::GetPlayerController(GetWorld(),0)->Possess(NewPlayer);
	}
	else
	{
		GameOver();
	}
}

//游戏分数
void AARPGGameGameModeBase::ScoreAdd(int val)
{
	if(val==10) ++KilledEmemy;
	++HitCount;
	if(HitCountText) HitCountText->SetText(FText::FromString(FString::FromInt(HitCount)));
	HitCountUserWidget->RemoveFromViewport();
	HitCountUserWidget->AddToViewport();
	Score+=val;
}

//在玩家附近随机产生敌人
void AARPGGameGameModeBase::SpawnEnemy()
{
	FVector PlayerLocation=UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetActorLocation();
	UNavigationSystemV1* NavMesh = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation RandomPt;
	NavMesh->GetRandomReachablePointInRadius(PlayerLocation,10000.f,RandomPt);
	if(UKismetMathLibrary::RandomBool()) GetWorld()->SpawnActor<ASwordAIChar>(ASwordAIChar::StaticClass(),RandomPt.Location,FRotator(0,0,0));
	else GetWorld()->SpawnActor<AArrowAIChar>(AArrowAIChar::StaticClass(),RandomPt.Location,FRotator(0,0,0));
}


//游戏结束
void AARPGGameGameModeBase::GameOver()
{
	CurrentWidget->RemoveFromViewport();
	CurrentWidget=CreateWidget(GetWorld(),GameOverWidget);
	if(CurrentWidget)
	{
		FinalScoreText=Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("TextBlock_3")));
		if(FinalScoreText) FinalScoreText->SetText(FText::FromString(FString::FromInt(Score)));
	}
	CurrentWidget->AddToViewport();
	IsGameOver=true;
}

//连击重置
void AARPGGameGameModeBase::HitCountReset()
{
	HitCount=0;
	HitCountFrame=0;
	HitCountSave=0;
	HitCountUserWidget->RemoveFromViewport();
}


//Called Every Frame

void AARPGGameGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//刷新屏幕显示参数
	APlayerChar* PC=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if(HPBarProgress) HPBarProgress->SetPercent(PC->CurrentHP/PC->MaxHP);
	if(ScoreText) ScoreText->SetText(FText::FromString(FString::FromInt(Score)));
	if(EnemyCountText) EnemyCountText->SetText(FText::FromString(FString::FromInt(KilledEmemy)));
	if(LivesText) LivesText->SetText(FText::FromString(FString::FromInt(Lives)));

	//连击判断
	if(HitCount==HitCountSave)
	{
		if(++HitCountFrame>600) HitCountReset();
	}
	else
	{
		HitCountFrame=0;
		HitCountSave=HitCount;
	}

	//敌人更新逻辑，每10s更新一次当前世界敌人数量
	if(EnemyNum<10)
	{
		++EnemyNum;
		SpawnEnemy();
	}
	float NowTime=UGameplayStatics::GetTimeSeconds(GetWorld());
	if((int)NowTime%10==0)
	{
		TArray<AActor*> AIChars;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),ASwordAIChar::StaticClass(),AIChars);
		int temp=AIChars.Num();
		AIChars.Empty();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),AArrowAIChar::StaticClass(),AIChars);
		EnemyNum=temp+AIChars.Num();
		AIChars.Empty();
	}
}

//游戏开始时调用
void AARPGGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//生成屏幕控件
	CurrentWidget=CreateWidget(GetWorld(),ScreenWidget);
	HitCountUserWidget=CreateWidget(GetWorld(),HitCountWidget);
	PostInitializeComponents();
	CurrentWidget->AddToViewport();
}

//初始化控件参数
void AARPGGameGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if(CurrentWidget)
	{
		HPBarProgress=Cast<UProgressBar>(CurrentWidget->GetWidgetFromName(TEXT("ProgressBar_0")));
		if(HPBarProgress) HPBarProgress->SetPercent(1.f);

		ScoreText=Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("Score")));
		if(ScoreText) ScoreText->SetText(FText::FromString(FString::FromInt(Score)));
		EnemyCountText=Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("EnemyCount")));
		if(EnemyCountText) EnemyCountText->SetText(FText::FromString(FString::FromInt(KilledEmemy)));
		LivesText=Cast<UTextBlock>(CurrentWidget->GetWidgetFromName(TEXT("LiveNumber")));
		if(LivesText) LivesText->SetText(FText::FromString(FString::FromInt(Lives)));
		
	}

	if(HitCountUserWidget)
	{
		HitCountText=Cast<UTextBlock>(HitCountUserWidget->GetWidgetFromName(TEXT("ComboText")));
		if(HitCountText) HitCountText->SetText(FText::FromString(FString::FromInt(HitCount)));
	}
}
