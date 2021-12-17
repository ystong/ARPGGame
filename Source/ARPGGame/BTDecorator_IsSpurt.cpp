// Created By Tong


#include "BTDecorator_IsSpurt.h"
#include "PlayerChar.h"
#include "SwordAIChar.h"
#include "SwordAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBTDecorator_IsSpurt::UBTDecorator_IsSpurt()
{
	bCreateNodeInstance=true;
}

bool UBTDecorator_IsSpurt::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//如果离玩家距离大于600，有5%的概率进行冲刺
	ASwordAIController* AIC=Cast<ASwordAIController>(OwnerComp.GetAIOwner());
	FVector AILocation=Cast<ASwordAIChar>(AIC->GetPawn())->GetActorLocation();
	FVector PlayerLocation=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0))->GetActorLocation();
	float Distance=UKismetMathLibrary::Vector_Distance(AILocation,PlayerLocation);
	if(UKismetMathLibrary::RandomFloat()<0.05 && Distance>600.f) return true;
	return false;
}
