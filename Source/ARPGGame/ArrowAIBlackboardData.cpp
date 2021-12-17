// Created By Tong

#include "ArrowAIBlackboardData.h"
#include "PlayerChar.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

void UArrowAIBlackboardData::PostLoad()
{
	Super::PostLoad();
	
	TargetPlayer.EntryName=FName("TargetPlayer");
	UBlackboardKeyType_Object* TargetPlayerKeyType=NewObject<UBlackboardKeyType_Object>();
	TargetPlayerKeyType->BaseClass=APlayerChar::StaticClass();
	TargetPlayer.KeyType=TargetPlayerKeyType;
	Keys.Add(TargetPlayer);
	
	Destination.EntryName=FName("Destination");
	UBlackboardKeyType_Vector* DestinationKeyType=NewObject<UBlackboardKeyType_Vector>();
	Destination.KeyType=DestinationKeyType;
	Keys.Add(Destination);

	PlayerLocation.EntryName=FName("PlayerLocation");
	UBlackboardKeyType_Vector* PlayerLocationKeyType=NewObject<UBlackboardKeyType_Vector>();
	PlayerLocation.KeyType=PlayerLocationKeyType;
	Keys.Add(PlayerLocation);

	LeavePlayerLocation.EntryName=FName("LeavePlayerLocation");
	UBlackboardKeyType_Vector* LeavePlayerLocationKeyType=NewObject<UBlackboardKeyType_Vector>();
	LeavePlayerLocation.KeyType=LeavePlayerLocationKeyType;
	Keys.Add(LeavePlayerLocation);
}
