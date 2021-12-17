// Created By Tong
// 弓箭手AI


#include "ArrowAIChar.h"
#include "ARPGGameGameModeBase.h"
#include "PlayerChar.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ArrowAIController.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"

// Sets default values
AArrowAIChar::AArrowAIChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//胶囊体初始化
	GetCapsuleComponent()->SetCapsuleHalfHeight(102.f);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);

	//网格体初始化
	ArrowAIMesh=CreateDefaultSubobject<USkeletalMesh>(TEXT("ArrowAIMesh"));
	ArrowAIMesh=LoadObject<USkeletalMesh>(NULL,TEXT("SkeletalMesh'/Game/AdamHorsemanArcher/Character/Adam/SK_Adam_Armored.SK_Adam_Armored'"));
	GetMesh()->SetSkeletalMesh(ArrowAIMesh);
	GetMesh()->Rename(TEXT("ArrowAIMeshComp"));
	GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f,-90.f,0.f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//武器系统加载
	LeftWeaponComp=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftWeaponComp"));
	RightWeaponComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWeaponComp"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> LeftWeaponMesh(TEXT("SkeletalMesh'/Game/AdamHorsemanArcher/Character/Adam/Bow/SK_AnimatedBow.SK_AnimatedBow'"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> RightWeaponMesh(TEXT("StaticMesh'/Game/AdamHorsemanArcher/Meshes/WP_Adam_Arrow.WP_Adam_Arrow'"));
	LeftWeaponComp->SetSkeletalMesh(LeftWeaponMesh.Object);
	RightWeaponComp->SetStaticMesh(RightWeaponMesh.Object);
	LeftWeaponComp->SetupAttachment(GetMesh(),TEXT("WeaponHandMount_lSocket"));
	RightWeaponComp->SetupAttachment(GetMesh(),TEXT("WeaponHandMount_rSocket"));
	RightWeaponComp->SetRelativeRotation(FRotator(59.1f,130.0f,19.9f));
	RightWeaponComp->SetRelativeLocation(FVector(-0.5315f,-1.2053f,0.1972f));
	LeftWeaponComp->SetRelativeRotation(FRotator(-65.1f,24.5f,43.0f));
	LeftWeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWeaponComp->SetVisibility(false);
	
	ArrowBallComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowBallComp"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowBallMesh(TEXT("StaticMesh'/Game/AdamHorsemanArcher/Meshes/WP_Adam_Quiver.WP_Adam_Quiver'"));
	ArrowBallComp->SetStaticMesh(ArrowBallMesh.Object);
	ArrowBallComp->SetupAttachment(GetMesh(),TEXT("SheldMount_backSocket"));
	ArrowBallComp->SetRelativeLocation(FVector(-3.7f,14.4f,-1.1f));
	ArrowBallComp->SetRelativeRotation(FRotator(-29.5f,164.3f,96.5f));
	ArrowBallComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ArrowStartComp=CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowStartComp"));
	ArrowStartComp->SetupAttachment(GetCapsuleComponent());
	ArrowStartComp->SetRelativeLocation(FVector(4.8f,26.6f,63.4f));
	//ArrowStartComp->SetRelativeRotation(FRotator(0.f,-5.f,0.f));
	

	//控件加载
	HPWidgetComp=CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidgetComp"));
	HPWidgetComp->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	UClass* HPWidgetClass=LoadClass<UUserWidget>(NULL,TEXT("Blueprint'/Game/UI/HPWidget.HPWidget_C'"));
	HPWidgetComp->SetWidgetClass(HPWidgetClass);
	HPWidgetComp->SetRelativeLocation(FVector(0.f,0.f,122.8f));
	HPWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	
	//绑定动画
	const ConstructorHelpers::FObjectFinder<UClass> AnimObj(TEXT("AnimBlueprint'/Game/AI/Arrow/ArrowAI_ABP.ArrowAI_ABP_C'"));
	GetMesh()->SetAnimInstanceClass(AnimObj.Object);
	
	const ConstructorHelpers::FObjectFinder<UAnimMontage> ArrowAIAttack_Montage(TEXT("AnimMontage'/Game/AdamHorsemanArcher/Animations/Adam/Archer/Archer_shot_Montage.Archer_shot_Montage'"));
	ArrowAIAttackMontage=ArrowAIAttack_Montage.Object;
	const ConstructorHelpers::FObjectFinder<UAnimMontage> ArrowAIAttack_Montage2(TEXT("AnimMontage'/Game/AdamHorsemanArcher/Animations/Adam/Archer/Acher_shot2_Montage.Acher_shot2_Montage'"));
	ArrowAIAttackMontage2=ArrowAIAttack_Montage2.Object;
	const ConstructorHelpers::FObjectFinder<UAnimMontage> Die_Montage(TEXT("AnimMontage'/Game/AdamHorsemanArcher/Animations/Adam/Archer/Dying_Montage.Dying_Montage'"));
	DieMontage=Die_Montage.Object;
	const ConstructorHelpers::FObjectFinder<UAnimMontage> BeHit_Montage(TEXT("AnimMontage'/Game/AdamHorsemanArcher/Animations/Adam/Archer/ArrowHitMontage.ArrowHitMontage'"));
	BeHitMontage=BeHit_Montage.Object;
	const ConstructorHelpers::FObjectFinder<UAnimMontage> Bow_Montage(TEXT("AnimMontage'/Game/AdamHorsemanArcher/Animations/Adam/Bow/Footman/Bow_charging_Montage.Bow_charging_Montage'"));
	BowMontage=Bow_Montage.Object;

	//基础属性
	MaxHP=50;
	CurrentHP=MaxHP;
	bIsDead=false;
	AttackValue=10;
	HitTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	GetCapsuleComponent()->BodyInstance.bLockXRotation=true;
	GetCapsuleComponent()->BodyInstance.bLockYRotation=true;
	GetCapsuleComponent()->BodyInstance.bLockZRotation=true;

	
	//绑定控制器
	AutoPossessAI=EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass=AArrowAIController::StaticClass();

}

// Called when the game starts or when spawned
void AArrowAIChar::BeginPlay()
{
	Super::BeginPlay();

	//初始化血量，随时间增强
	float NowTime=UGameplayStatics::GetTimeSeconds(GetWorld());
	if(NowTime<600)
	{
		MaxHP=50+NowTime/6;
	}
	else
	{
		MaxHP=150;
	}
	CurrentHP=MaxHP;
	
	PostInitializeComponents();
}

// Called every frame
void AArrowAIChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//随时间增强攻击力
	float NowTime=UGameplayStatics::GetTimeSeconds(GetWorld());
	if(NowTime<600)
	{
		AttackValue=10+NowTime/10;
	}
	else
	{
		AttackValue=70;
	}

	//如果世界Z坐标小于-10000，销毁Actor（掉出世界平台）
	if(GetActorLocation().Z<-10000)
	{
		Cast<AARPGGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->EnemyNum--;
		K2_DestroyActor();
	}
}

// Called to bind functionality to input
void AArrowAIChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//初始化血条控件
void AArrowAIChar::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UUserWidget* CurrentWidget=HPWidgetComp->GetUserWidgetObject();
	if(CurrentWidget)
	{
		HPBarProgress=Cast<UProgressBar>(CurrentWidget->GetWidgetFromName(TEXT("HPBarProgress")));
		if(HPBarProgress) HPBarProgress->SetPercent(CurrentHP/MaxHP);
	}
}

//死亡延迟回调
void AArrowAIChar::DieCallBack()
{
	K2_DestroyActor();
	Cast<AARPGGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->EnemyNum--;
}

//受击延迟回调
void AArrowAIChar::HitCallBack()
{
	Cast<AArrowAIController>(GetController())->RunBT();
}

//播放拉弓动画
void AArrowAIChar::DoAttack()
{
	PlayAnimMontage(ArrowAIAttackMontage,1.0);
	struct FLatentActionInfo LatentInfo;
	LatentInfo.Linkage=0;
	LatentInfo.CallbackTarget = this;
	LatentInfo.UUID = __LINE__;
	LatentInfo.ExecutionFunction = "ShotCallBack";
	UKismetSystemLibrary::Delay(GetWorld(),1.56f,LatentInfo);
}

//播放箭矢射出动画
void AArrowAIChar::ShotCallBack()
{
	PlayAnimMontage(ArrowAIAttackMontage2);
}

//播放弓变形动画
void AArrowAIChar::BowCallBack()
{
	//LeftWeaponComp->GetAnimInstance()->Montage_Play(BowMontage);
}

//受到伤害逻辑
float AArrowAIChar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
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
		Cast<AArrowAIController>(GetController())->StopBT();
		struct FLatentActionInfo LatentInfo;
		LatentInfo.Linkage=0;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = "HitCallBack";
		LatentInfo.UUID = __LINE__;
		UKismetSystemLibrary::Delay(GetWorld(),t,LatentInfo);
	}
	return 0.f;
}

