// Created By Tong
// 箭矢Actor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowActor.generated.h"

UCLASS()
class ARPGGAME_API AArrowActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrowActor();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* ArrowComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UProjectileMovementComponent* PMComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class AArrowAIChar* FatherAI;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GetSpawnAI(class AArrowAIChar* ArrowAI);

};
