// Created By Tong


#include "BTService_AtGoal.h"
#include "SwordAIController.h"
#include "SwordAIChar.h"
#include "NavigationSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerChar.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

UBTService_AtGoal::UBTService_AtGoal()
{
	bCreateNodeInstance=true;
}

void UBTService_AtGoal::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//对象的多球体检测，200范围内是否有玩家
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ASwordAIController* AIC=Cast<ASwordAIController>(OwnerComp.GetAIOwner());
	ASwordAIChar* SwordAI=Cast<ASwordAIChar>(AIC->GetPawn());
	FVector AILocation=Cast<ASwordAIChar>(AIC->GetPawn())->GetActorLocation();
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
		200.0,
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
			//如果撞到玩家，开启玩家物理模拟，并添加AI冲刺方向的冲量
			PC->GetCapsuleComponent()->SetSimulatePhysics(true);
			FVector SwordAIFV=SwordAI->GetCapsuleComponent()->GetForwardVector();
			SwordAIFV.X*=100000;
			SwordAIFV.Y*=100000;
			SwordAIFV.Z=50000;
			PC->GetCapsuleComponent()->AddImpulseAtLocation(SwordAIFV,PC->GetActorLocation());
				
			struct FLatentActionInfo LatentInfo;
			LatentInfo.Linkage=0;
			LatentInfo.CallbackTarget = this;
			LatentInfo.ExecutionFunction = "DoFlyCallBack";
			LatentInfo.UUID = __LINE__;
			UKismetSystemLibrary::Delay(GetWorld(),1.f,LatentInfo);
		}
	}
}

//击飞后延迟1s将玩家物理模拟关闭
void UBTService_AtGoal::DoFlyCallBack()
{
	APlayerChar*  PC=Cast<APlayerChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	PC->GetCapsuleComponent()->SetSimulatePhysics(false);
}
