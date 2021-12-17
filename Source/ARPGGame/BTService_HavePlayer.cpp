// Created By Tong


#include "BTService_HavePlayer.h"
#include "SwordAIController.h"
#include "SwordAIChar.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerChar.h"
#include "BehaviorTree/BTFunctionLibrary.h"

UBTService_HavePlayer::UBTService_HavePlayer()
{
	bCreateNodeInstance=true;
}

void UBTService_HavePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//对象的多球体检测，2000范围内是否有玩家
	ASwordAIController* AIC=Cast<ASwordAIController>(OwnerComp.GetAIOwner());
	if(AIC!=nullptr)
	{
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
			2000.0,
			HitTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
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
				//如果有玩家，将玩家和玩家位置设置为黑板键
				UBTFunctionLibrary::SetBlackboardValueAsObject(GetNodeInstance(OwnerComp,NodeMemory),TargetPlayer,PC);
				UBTFunctionLibrary::SetBlackboardValueAsVector(GetNodeInstance(OwnerComp,NodeMemory),PlayerLocation,PC->GetActorLocation());
			}
		}
	}
}
