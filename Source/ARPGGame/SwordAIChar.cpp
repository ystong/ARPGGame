// Created By Tong


#include "SwordAIChar.h"
#include "ARPGGameGameModeBase.h"
#include "ArrowAIChar.h"
#include "PlayerChar.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SwordAIController.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASwordAIChar::ASwordAIChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//胶囊体初始化
	GetCapsuleComponent()->SetCapsuleHalfHeight(92.f);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);

	//网格体初始化
	SwordAIMesh=CreateDefaultSubobject<USkeletalMesh>(TEXT("SwordAIMesh"));
	SwordAIMesh=LoadObject<USkeletalMesh>(NULL,TEXT("SkeletalMesh'/Game/AdamHorsemanArcher/Character/Adam/SK_Adam_Armored.SK_Adam_Armored'"));
	GetMesh()->SetSkeletalMesh(SwordAIMesh);
	GetMesh()->Rename(TEXT("SwordAIMeshComp"));
	GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f,-90.f,0.f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//武器系统加载
	LeftWeaponComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWeaponComp"));
	RightWeaponComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWeaponComp"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> LeftWeaponMesh(TEXT("StaticMesh'/Game/AdamHorsemanArcher/Meshes/WP_Adam_Shield.WP_Adam_Shield'"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> RightWeaponMesh(TEXT("StaticMesh'/Game/AdamHorsemanArcher/Meshes/WP_Adam_Scimitar.WP_Adam_Scimitar'"));
	LeftWeaponComp->SetStaticMesh(LeftWeaponMesh.Object);
	RightWeaponComp->SetStaticMesh(RightWeaponMesh.Object);
	LeftWeaponComp->SetupAttachment(GetMesh(),TEXT("WeaponHandMount_lSocket"));
	RightWeaponComp->SetupAttachment(GetMesh(),TEXT("WeaponHandMount_rSocket"));
	RightWeaponComp->SetRelativeRotation(FRotator(-25.7f,73.9f,-56.3f));
	LeftWeaponComp->SetRelativeRotation(FRotator(-80.f,10.f,-180.f));
	LeftWeaponComp->SetRelativeLocation(FVector(1.654f,0.292f,-3.766f));
	LeftWeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//控件加载
	HPWidgetComp=CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidgetComp"));
	HPWidgetComp->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	UClass* HPWidgetClass=LoadClass<UUserWidget>(NULL,TEXT("Blueprint'/Game/UI/HPWidget.HPWidget_C'"));
	HPWidgetComp->SetWidgetClass(HPWidgetClass);
	HPWidgetComp->SetRelativeLocation(FVector(0.f,0.f,122.8f));
	HPWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	
	//绑定动画
	const ConstructorHelpers::FObjectFinder<UClass> AnimObj(TEXT("AnimBlueprint'/Game/AI/Sword/SwordAIAnimBP.SwordAIAnimBP_C'"));
	GetMesh()->SetAnimInstanceClass(AnimObj.Object);
	const ConstructorHelpers::FObjectFinder<UAnimMontage> SwordAIAttack_Montage(TEXT("AnimMontage'/Game/AdamHorsemanArcher/Animations/Adam/WeaponAndShield/1H_sword_swing_high_right_Montage.1H_sword_swing_high_right_Montage'"));
	SwordAIAttackMontage=SwordAIAttack_Montage.Object;
	const ConstructorHelpers::FObjectFinder<UAnimMontage> Die_Montage(TEXT("AnimMontage'/Game/AdamHorsemanArcher/Animations/Adam/WeaponAndShield/SwordAIDieAnim.SwordAIDieAnim'"));
	DieMontage=Die_Montage.Object;
	const ConstructorHelpers::FObjectFinder<UAnimMontage> BeHit_Montage(TEXT("AnimMontage'/Game/AdamHorsemanArcher/Animations/Adam/WeaponAndShield/SwordAIHitMontage.SwordAIHitMontage'"));
	BeHitMontage=BeHit_Montage.Object;

	//基础属性
	MaxHP=100;
	CurrentHP=MaxHP;
	bIsDead=false;
	AttackValue=5;
	HitTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	GetCapsuleComponent()->BodyInstance.bLockXRotation=true;
	GetCapsuleComponent()->BodyInstance.bLockYRotation=true;
	GetCapsuleComponent()->BodyInstance.bLockZRotation=true;

	
	//绑定控制器
	AutoPossessAI=EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass=ASwordAIController::StaticClass();
}

void ASwordAIChar::DoAttack()
{
	PlayAnimMontage(SwordAIAttackMontage,1.0);
}

// Called when the game starts or when spawned
void ASwordAIChar::BeginPlay()
{
	Super::BeginPlay();
	
	//初始化血量，随时间增强
	float NowTime=UGameplayStatics::GetTimeSeconds(GetWorld());
	if(NowTime<600)
	{
		MaxHP=100+NowTime/6;
	}
	else
	{
		MaxHP=200;
	}
	CurrentHP=MaxHP;

	PostInitializeComponents();
	
}

// Called every frame
void ASwordAIChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//随时间增强攻击力
	float NowTime=UGameplayStatics::GetTimeSeconds(GetWorld());
	if(NowTime<600)
	{
		AttackValue=5+NowTime/12;
	}
	else
	{
		AttackValue=55;
	}

	//如果世界Z坐标小于-10000，销毁Actor（掉出世界平台）
	if(GetActorLocation().Z<-10000)
	{
		Cast<AARPGGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->EnemyNum--;
		K2_DestroyActor();
	}
}

// Called to bind functionality to input
void ASwordAIChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//初始化血条控件
void ASwordAIChar::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UUserWidget* CurrentWidget=HPWidgetComp->GetUserWidgetObject();
	if(CurrentWidget)
	{
		HPBarProgress=Cast<UProgressBar>(CurrentWidget->GetWidgetFromName(TEXT("HPBarProgress")));
		if(HPBarProgress) HPBarProgress->SetPercent(CurrentHP/MaxHP);
	}
}

//攻击检测
void ASwordAIChar::DoHit()
{
	FVector StartLocation=GetActorLocation();
	const FVector ForwardVector=UKismetMathLibrary::GetForwardVector(GetActorRotation());
	FVector EndLocation=ForwardVector*150+StartLocation;
	StartLocation+=ForwardVector*100;
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutResult;
	TSubclassOf<UDamageType> DamageClass;

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		StartLocation,
		EndLocation,
		75.0,
		HitTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		OutResult,
		true,
		FColor::Red,
		FColor::Green,
		5.0
	);
	for(int i=0;i<OutResult.Num();++i)
	{
		AActor* HitActor=OutResult[i].GetActor();
		APlayerChar* PC=Cast<APlayerChar>(HitActor);
		if(PC) UGameplayStatics::ApplyDamage(PC,AttackValue+UKismetMathLibrary::RandomFloat()*2, GetController(),this,DamageClass);
	}
}

//死亡延迟回调
void ASwordAIChar::DieCallBack()
{
	K2_DestroyActor();
	Cast<AARPGGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->EnemyNum--;
}

//受击延迟回调
void ASwordAIChar::HitCallBack()
{
	Cast<ASwordAIController>(GetController())->RunBT();
}

//受伤逻辑
float ASwordAIChar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                               AActor* DamageCauser)
{
	if(CurrentHP-DamageAmount<=0)
	{
		CurrentHP=0.f;
		HPBarProgress->SetPercent(CurrentHP/MaxHP);
		bIsDead=true;
		Cast<AARPGGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->ScoreAdd(10);
		GetCapsuleComponent()->DestroyComponent();
		float t=PlayAnimMontage(DieMontage,1.0);
		struct FLatentActionInfo LatentInfo;
		LatentInfo.Linkage=0;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = "DieCallBack";
		LatentInfo.UUID = __LINE__;
		UKismetSystemLibrary::Delay(GetWorld(),t,LatentInfo);
	}
	else
	{
		CurrentHP-=DamageAmount;
		Cast<AARPGGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->ScoreAdd(2);
		HPBarProgress->SetPercent(CurrentHP/MaxHP);
		float t=PlayAnimMontage(BeHitMontage,1.0);
		Cast<ASwordAIController>(GetController())->StopBT();
		struct FLatentActionInfo LatentInfo;
		LatentInfo.Linkage=0;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = "HitCallBack";
		LatentInfo.UUID = __LINE__;
		UKismetSystemLibrary::Delay(GetWorld(),t,LatentInfo);
	}
	return 0.f;
}

