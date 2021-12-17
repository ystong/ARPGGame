// Created By Tong


#include "ArrowActor.h"
#include "ArrowAIChar.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerChar.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AArrowActor::AArrowActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//碰撞框及网格设置
	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComp->InitBoxExtent(FVector(4.f,2.f,2.f));
	BoxComp->SetupAttachment(RootComponent);
	
	ArrowComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMesh(TEXT("StaticMesh'/Game/AdamHorsemanArcher/Meshes/WP_Adam_Arrow.WP_Adam_Arrow'"));
	ArrowComp->SetStaticMesh(ArrowMesh.Object);
	ArrowComp->SetupAttachment(BoxComp);
	ArrowComp->SetRelativeLocation(FVector(-78.4f,0.f,0.f));

	//初始速度
	PMComp=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PM"));
	PMComp->InitialSpeed=3000;
	
}

// Called when the game starts or when spawned
void AArrowActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrowActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//射出后进行多球体检测
	FVector AILocation=BoxComp->GetComponentLocation();
	FVector EndLocation=AILocation+FVector(1.f,1.f,1.f);
		
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutResult;
	TSubclassOf<UDamageType> DamageClass;

	TArray< TEnumAsByte< EObjectTypeQuery > > HitTypes;
	HitTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		AILocation,
		EndLocation,
		2.0,
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
		if(PC)
		{
			UGameplayStatics::ApplyDamage(PC,FatherAI->AttackValue,FatherAI->GetController(),FatherAI,DamageClass);
			//一旦命中就销毁Actor
			K2_DestroyActor();
		}
	}
}

//获取射出自己的AI
void AArrowActor::GetSpawnAI(AArrowAIChar* ArrowAI)
{
	FatherAI=ArrowAI;
}


