// Created By Tong


#include "PlayerChar.h"
#include "ARPGGameGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "SwordAIChar.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

//物理表面定义
#define SURFACE_GROSS SurfaceType1

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//胶囊体初始化
	GetCapsuleComponent()->SetCapsuleHalfHeight(92.f);

	//摄像头初始化
	SpringArmComp=CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp=CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	CameraComp->SetupAttachment(SpringArmComp);
	SpringArmComp->bUsePawnControlRotation=true;
	SpringArmComp->bEnableCameraLag=true;
	
	
	//网格体初始化
	PlayerMesh=CreateDefaultSubobject<USkeletalMesh>(TEXT("PlayerMesh"));
	PlayerMesh=LoadObject<USkeletalMesh>(NULL,TEXT("SkeletalMesh'/Game/SnSAnimsetPro/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	GetMesh()->SetSkeletalMesh(PlayerMesh);
	GetMesh()->Rename(TEXT("PlayerMeshComp"));
	GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f,-90.f,0.f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//武器系统加载
	LeftWeaponComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWeaponComp"));
	RightWeaponComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWeaponComp"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> LeftWeaponMesh(TEXT("StaticMesh'/Game/SnSAnimsetPro/Models/Shield/Shield.Shield'"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> RightWeaponMesh(TEXT("StaticMesh'/Game/SnSAnimsetPro/Models/Sword/Sword.Sword'"));
	LeftWeaponComp->SetStaticMesh(LeftWeaponMesh.Object);
	RightWeaponComp->SetStaticMesh(RightWeaponMesh.Object);
	LeftWeaponComp->SetupAttachment(GetMesh(),TEXT("lowerarm_lSocket"));
	RightWeaponComp->SetupAttachment(GetMesh(),TEXT("hand_rSocket"));
	LeftWeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	NiagaraComp=CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	//const ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSys(TEXT("NiagaraSystem'/Game/Niagara/SwordNiagaraEmitter_System.SwordNiagaraEmitter_System'"));
	SwordES=LoadObject<UNiagaraSystem>(NULL,TEXT("NiagaraSystem'/Game/Niagara/SwordNiagaraEmitter_System.SwordNiagaraEmitter_System'"));
	NiagaraComp->SetAsset(SwordES);
	NiagaraComp->SetupAttachment(RightWeaponComp,TEXT("NiagaraSocket"));
	NiagaraComp->SetVisibility(false);

	//视角相关设置
	GetCharacterMovement()->bOrientRotationToMovement=true;
	bUseControllerRotationYaw=false;
	GetCharacterMovement()->RotationRate=FRotator(0.0f,1000.0f,0.f);

	//绑定动画
	const ConstructorHelpers::FObjectFinder<UClass> AnimObj(TEXT("AnimBlueprint'/Game/PlayerChar/PlayerCharAnim_BP.PlayerCharAnim_BP_C'"));
	GetMesh()->SetAnimInstanceClass(AnimObj.Object);

	//赋予初始属性
	bIsAttack=false;
	AttackCount=-1;
	bSaveAttack=false;
	bHasMovementInput=false;
	bIsRoll=false;
	bIsJump=false;
	MaxHP=100;
	CurrentHP=100;
	bIsDead=false;
	GetCharacterMovement()->MaxWalkSpeed=500.f;
	HitTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	const ConstructorHelpers::FObjectFinder<UAnimMontage> Combo_01_Montage(TEXT("AnimMontage'/Game/SnSAnimsetPro/Animations/RootMotion/Combo_01_Montage.Combo_01_Montage'"));
	const ConstructorHelpers::FObjectFinder<UAnimMontage> Combo_02_Montage(TEXT("AnimMontage'/Game/SnSAnimsetPro/Animations/RootMotion/Combo_02_Montage.Combo_02_Montage'"));
	const ConstructorHelpers::FObjectFinder<UAnimMontage> Combo_03_Montage(TEXT("AnimMontage'/Game/SnSAnimsetPro/Animations/RootMotion/Combo_03_Montage.Combo_03_Montage'"));
	const ConstructorHelpers::FObjectFinder<UAnimMontage> Combo_04_Montage(TEXT("AnimMontage'/Game/SnSAnimsetPro/Animations/RootMotion/Combo_04_Montage.Combo_04_Montage'"));
	const ConstructorHelpers::FObjectFinder<UAnimMontage> Combo_05_Montage(TEXT("AnimMontage'/Game/SnSAnimsetPro/Animations/RootMotion/Combo_05_Montage.Combo_05_Montage'"));
	AttackMontage.Add(Combo_01_Montage.Object);
	AttackMontage.Add(Combo_02_Montage.Object);
	AttackMontage.Add(Combo_03_Montage.Object);
	AttackMontage.Add(Combo_04_Montage.Object);
	AttackMontage.Add(Combo_05_Montage.Object);

	const ConstructorHelpers::FObjectFinder<UMaterial> Skin01(TEXT("Material'/DatasmithContent/Materials/FBXImporter/VRED/BrushedMetal.BrushedMetal'"));
	const ConstructorHelpers::FObjectFinder<UMaterial> Skin02(TEXT("Material'/Game/SnSAnimsetPro/UE4_Mannequin/Materials/M_UE4Man_Body.M_UE4Man_Body'"));
	const ConstructorHelpers::FObjectFinder<UMaterial> Skin03(TEXT("Material'/Game/Tropical_Forest/trees/Alsophilia/Materials/Alsophili_bark2.Alsophili_bark2'"));
	const ConstructorHelpers::FObjectFinder<UMaterial> Skin04(TEXT("Material'/Engine/VREditor/LaserPointer/LaserPointerMaterial.LaserPointerMaterial'"));
	const ConstructorHelpers::FObjectFinder<UMaterial> Skin05(TEXT("Material'/Engine/EditorMaterials/PersonaFloorMat.PersonaFloorMat'"));
	const ConstructorHelpers::FObjectFinder<UMaterial> Skin06(TEXT("Material'/Engine/EditorMaterials/WidgetGridVertexColorMaterial_Ma.WidgetGridVertexColorMaterial_Ma'"));
	SkinArray.Add(Skin01.Object);
	SkinArray.Add(Skin02.Object);
	SkinArray.Add(Skin03.Object);
	SkinArray.Add(Skin04.Object);
	SkinArray.Add(Skin05.Object);
	SkinArray.Add(Skin06.Object);

	const ConstructorHelpers::FObjectFinder<UAnimMontage> Rolling_Montage(TEXT("AnimMontage'/Game/SnSAnimsetPro/Animations/RootMotion/Rolling_Front_Montage.Rolling_Front_Montage'"));
	RollMontage=Rolling_Montage.Object;
	const ConstructorHelpers::FObjectFinder<UAnimMontage> Jump_Montage(TEXT("AnimMontage'/Game/SnSAnimsetPro/Animations/RootMotion/Jump_Montage.Jump_Montage'"));
	JumpMontage=Jump_Montage.Object;
	const ConstructorHelpers::FObjectFinder<UAnimMontage> Die_Montage(TEXT("AnimMontage'/Game/SnSAnimsetPro/Animations/RootMotion/Die_Montage.Die_Montage'"));
	DieMontage=Die_Montage.Object;
	const ConstructorHelpers::FObjectFinder<UAnimMontage> BeHit_Montage(TEXT("AnimMontage'/Game/SnSAnimsetPro/Animations/InPlace/Damage_MOntage.Damage_Montage'"));
	BeHitMontage=BeHit_Montage.Object;
	BleedES=LoadObject<UNiagaraSystem>(NULL,TEXT("NiagaraSystem'/Game/Niagara/BleedNiagaraEmitter_System.BleedNiagaraEmitter_System'"));
	DeadES=LoadObject<UNiagaraSystem>(NULL,TEXT("NiagaraSystem'/Game/Niagara/DiedNiagaraEmitter_System.DiedNiagaraEmitter_System'"));

	//基本物理
	GetCapsuleComponent()->BodyInstance.bLockXRotation=true;
	GetCapsuleComponent()->BodyInstance.bLockYRotation=true;
	GetCapsuleComponent()->BodyInstance.bLockZRotation=true;
	
	//获取控制权
	AutoPossessPlayer=EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//对象的多射线检测，检测当前地形表面属性
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutResult;
	TSubclassOf<UDamageType> DamageClass;
	TArray< TEnumAsByte< EObjectTypeQuery > > SurfaceType;
	SurfaceType.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

	UKismetSystemLibrary::LineTraceMultiForObjects(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation()+FVector(0.f,0.f,-200),
		SurfaceType,
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
		EPhysicalSurface Surface=UGameplayStatics::GetSurfaceType(OutResult[i]);
		if(Surface)
		{
			//如果当前地形是草地，则减少200行走速度
			float WalkSpeed=GetCharacterMovement()->MaxWalkSpeed;
			if(Surface==SURFACE_GROSS)
			{
				if(WalkSpeed==500 || WalkSpeed==1000)
				{
					GetCharacterMovement()->MaxWalkSpeed-=200;
				}
			}
			//如果是其他地形并且行走速度已经衰减，就还原行走速度
			else if(WalkSpeed==300 ||  WalkSpeed==800)
			{
				GetCharacterMovement()->MaxWalkSpeed+=200;
			}
		}
	}
}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

//移动
void APlayerChar::MoveForward(float val)
{
	if(!bIsDead) AddMovementInput(UKismetMathLibrary::GetForwardVector(GetControlRotation()),val);
}

void APlayerChar::MoveRight(float val)
{
	if(!bIsDead) AddMovementInput(UKismetMathLibrary::GetRightVector(GetControlRotation()),val);	
}

//视角
void APlayerChar::LookUp(float val)
{
	AddControllerPitchInput(val);
}

void APlayerChar::LookRight(float val)
{
	AddControllerYawInput(val);
}

//跳跃
void APlayerChar::DoJump()
{
	if(bIsDead || bIsAttack || bIsJump || bIsRoll){}
	else
	{
		bIsJump=true;
		ComboReset();
		float t=PlayAnimMontage(JumpMontage,1.0);
		struct FLatentActionInfo LatentInfo;
		LatentInfo.Linkage=0;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = "DoJumpCallback";
		LatentInfo.UUID = __LINE__;
		UKismetSystemLibrary::Delay(GetWorld(),t,LatentInfo);
	}
}

//跳跃回调
void APlayerChar::DoJumpCallBack()
{
	bIsJump=false;
	this->ComboReset();
}

//翻滚回调
void APlayerChar::DoRollCallBack()
{
	bIsRoll=false;
	this->ComboReset();
}

//攻击
void APlayerChar::DoAttack()
{
	if(!bIsDead)
	{
		GetCapsuleComponent()->SetSimulatePhysics(false);
	}
	if(bIsDead || bIsAttack || bIsJump || bIsRoll)
	{
		bSaveAttack=true;
	}
	else
	{
		bIsAttack=true;
		NiagaraComp->SetVisibility(true);
		++AttackCount;
		PlayAnimMontage(AttackMontage[AttackCount],1.0);
		if(AttackCount+1>=AttackMontage.Num())
		{
			AttackCount=-1;
		}
	}
}

//保存连击状态
void APlayerChar::AttackSave()
{
	if(bSaveAttack)
	{
		bSaveAttack=false;
		++AttackCount;
		PlayAnimMontage(AttackMontage[AttackCount],1.0);
		if(AttackCount+1>=AttackMontage.Num())
		{
			AttackCount=-1;
		}
	}
}

//重置连击状态
void APlayerChar::ComboReset()
{
	AttackCount=-1;
	bIsAttack=false;
	NiagaraComp->SetVisibility(false);
	bSaveAttack=false;
}

//向前翻滚
void APlayerChar::DoRoll()
{
	if(bIsDead||bIsRoll){}
	else
	{
		bIsRoll=true;
		ComboReset();
		float t=PlayAnimMontage(RollMontage,1.0);
		struct FLatentActionInfo LatentInfo;
		LatentInfo.Linkage=0;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = "DoRollCallBack";
		LatentInfo.UUID = __LINE__;
		UKismetSystemLibrary::Delay(GetWorld(),t,LatentInfo);
	}
}

//攻击检测
void APlayerChar::DoHit()
{
	FVector StartLocation=GetActorLocation();
	const FVector ForwardVector=UKismetMathLibrary::GetForwardVector(GetActorRotation());
	FVector EndLocation=ForwardVector*200+StartLocation;
	StartLocation+=ForwardVector*99;
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutResult;
	TSubclassOf<UDamageType> DamageClass;

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		StartLocation,
		EndLocation,
		100.0,
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
		if(PC){}
		else
		{
			UGameplayStatics::ApplyDamage(HitActor,10+UKismetMathLibrary::RandomFloat()*10, GetController(),this,DamageClass);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),BleedES,HitActor->GetActorLocation());
		}
	}
}

//死亡回调
void APlayerChar::DieCallBack()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),DeadES,GetMesh()->GetComponentLocation(),GetMesh()->GetComponentRotation());
	
	GetMesh()->SetVisibility(false);
	LeftWeaponComp->SetVisibility(false);
	RightWeaponComp->SetVisibility(false);
	
	struct FLatentActionInfo LatentInfo;
	LatentInfo.Linkage=0;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "ReliveCallBack";
	LatentInfo.UUID = __LINE__;
	UKismetSystemLibrary::Delay(GetWorld(),5.f,LatentInfo);
	
}

//IK射线检测
float APlayerChar::IKLineTrace(FName FootName,float Distance)
{
	FVector TraceLocation=GetMesh()->GetSocketLocation(FootName);;
	TraceLocation.Z=GetMesh()->GetComponentLocation().Z;
	FVector StartLocation=TraceLocation+FVector(0,0,50.f);
	FVector EndLocation=TraceLocation-FVector(0,0,Distance);
	TArray<AActor*> ActorsToIgnore;
	FHitResult OutResult;
	TSubclassOf<UDamageType> DamageClass;
	
	bool IKHit=GetWorld()->LineTraceSingleByChannel(OutResult,StartLocation,EndLocation,ECollisionChannel::ECC_Visibility);
	float HitDis=OutResult.Distance;
	if(IKHit) return 50.f-HitDis;
	return 0;
}

//皮肤更换
void APlayerChar::ChangeMeshSkin()
{
	GetMesh()->SetMaterial(0,SkinArray[UKismetMathLibrary::RandomInteger(6)]);
}

void APlayerChar::ChangeChestSkin()
{
	GetMesh()->SetMaterial(1,SkinArray[UKismetMathLibrary::RandomInteger(6)]);
}

void APlayerChar::ChangeShieldSkin()
{
	LeftWeaponComp->SetMaterial(0,SkinArray[UKismetMathLibrary::RandomInteger(6)]);
}

void APlayerChar::ChangeSwordSkin()
{
	RightWeaponComp->SetMaterial(0,SkinArray[UKismetMathLibrary::RandomInteger(6)]);
}

void APlayerChar::ChangeAllSkin()
{
	ChangeChestSkin();
	ChangeMeshSkin();
	ChangeShieldSkin();
	ChangeSwordSkin();
}

//复活回调
void APlayerChar::ReliveCallBack()
{
	Cast<AARPGGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->ReLive();
	
	if(Cast<AARPGGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->Lives>0) K2_DestroyActor();
	else
	{
		GetMesh()->SetVisibility(false);
		LeftWeaponComp->SetVisibility(false);
		RightWeaponComp->SetVisibility(false);
	}
}

//受到伤害
float APlayerChar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                              AActor* DamageCauser)
{
	if(CurrentHP-DamageAmount<=0)
	{
		CurrentHP=0.f;
		bIsDead=true;
		Cast<AARPGGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->HitCountReset();
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
		ComboReset();
		Cast<AARPGGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->HitCountReset();
		float t=PlayAnimMontage(BeHitMontage,1.0);
		struct FLatentActionInfo LatentInfo;
		LatentInfo.Linkage=0;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = "HitCallBack";
		LatentInfo.UUID = __LINE__;
		UKismetSystemLibrary::Delay(GetWorld(),t,LatentInfo);
	}
	return 0.f;
}



