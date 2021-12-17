// Created By Tong


#include "BTDecorator_CloseToPlayer.h"
#include "PlayerChar.h"
#include "ArrowAIController.h"
#include "ArrowAIChar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"


UBTDecorator_CloseToPlayer::UBTDecorator_CloseToPlayer()
{
	bCreateNodeInstance=true;
}

bool UBTDecorator_CloseToPlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	//对象的多球体检测，500范围内是否有玩家
	AArrowAIController* AIC=Cast<AArrowAIController>(OwnerComp.GetAIOwner());
	if(AIC!=nullptr)
	{
		FVector AILocation=Cast<AArrowAIChar>(AIC->GetPawn())->GetActorLocation();
		FVector EndLocation=AILocation+FVector(1.f,1.f,1.f);
		AArrowAIChar* ArrowAI=Cast<AArrowAIChar>(AIC->GetPawn());
		
		TArray<AActor*> ActorsToIgnore;
		TArray<FHitResult> OutResult;
		TSubclassOf<UDamageType> DamageClass;

		TArray< TEnumAsByte< EObjectTypeQuery > > HitTypes;
		HitTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			AILocation,
			EndLocation,
			500.0,
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
				//如果有玩家 则将远处位置设置为黑板键
				FVector LeaveLocation=PC->GetActorLocation()-ArrowAI->GetActorLocation();
				LeaveLocation.X*=-1;
				LeaveLocation.Y*=-1;
				LeaveLocation+=ArrowAI->GetActorLocation();
				UBTFunctionLibrary::SetBlackboardValueAsVector(GetNodeInstance(OwnerComp,NodeMemory),LeavePlayerLocation,LeaveLocation);
			}
		}
		//判断是否有正在进行的蒙太奇
		if(ArrowAI->GetMesh()->GetAnimInstance()->Montage_IsPlaying(NULL)) return false;
		return true;
	}
	return false;
}