// Created By Tong
// 弓箭手AI

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArrowAIChar.generated.h"

UCLASS()
class ARPGGAME_API AArrowAIChar : public ACharacter
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsDead;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AttackValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* DieMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* BeHitMontage;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ArrowAIMesh")
	class USkeletalMesh* ArrowAIMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SpringArmComp")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="CameraComp")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LeftWeaponComp")
	class USkeletalMeshComponent* LeftWeaponComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="RifhtWeaponComp")
	class UStaticMeshComponent* RightWeaponComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ArrowBallComp")
	class UStaticMeshComponent* ArrowBallComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ArrowStartComp")
	class UArrowComponent*ArrowStartComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HPWidgetComp")
	class UWidgetComponent* HPWidgetComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* ArrowAIAttackMontage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* ArrowAIAttackMontage2;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* BowMontage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray< TEnumAsByte< EObjectTypeQuery > > HitTypes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UProgressBar* HPBarProgress;


public:
	// Sets default values for this character's properties
	AArrowAIChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable)
	void DieCallBack();
	UFUNCTION(BlueprintCallable)
	void HitCallBack();
	UFUNCTION(BlueprintCallable)
	void DoAttack();
	UFUNCTION(BlueprintCallable)
	void ShotCallBack();
	UFUNCTION(BlueprintCallable)
	void BowCallBack();
};
