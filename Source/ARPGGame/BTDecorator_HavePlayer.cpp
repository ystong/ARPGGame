// Created By Tong


#include "BTDecorator_HavePlayer.h"
#include "PlayerChar.h"
#include "SwordAIController.h"
#include "SwordAIChar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"


UBTDecorator_HavePlayer::UBTDecorator_HavePlayer()
{
	bCreateNodeInstance=true;
}

bool UBTDecorator_HavePlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//对象的多球体检测 判断2000范围内是否出现玩家
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
				//如果有玩家，则将玩家和玩家位置设置为黑板键
				UBTFunctionLibrary::SetBlackboardValueAsObject(GetNodeInstance(OwnerComp,NodeMemory),TargetPlayer,PC);
				UBTFunctionLibrary::SetBlackboardValueAsVector(GetNodeInstance(OwnerComp,NodeMemory),PlayerLocation,PC->GetActorLocation());
			}
		}
		//判断是否有正在进行的蒙太奇
		ASwordAIChar* SwordAI=Cast<ASwordAIChar>(AIC->GetPawn());
		if(SwordAI->GetMesh()->GetAnimInstance()->Montage_IsPlaying(NULL)) return false;
		return true;
	}
	return false;

}
