// Created By Tong
// 玩家类

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerChar.generated.h"

UCLASS()
class ARPGGAME_API APlayerChar : public ACharacter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsAttack;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bSaveAttack;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bHasMovementInput;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsRoll;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsJump;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsDead;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UAnimMontage*> AttackMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UMaterial*> SkinArray;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* RollMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* JumpMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* SkillMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* DieMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* BeHitMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray< TEnumAsByte< EObjectTypeQuery > > HitTypes;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 AttackCount;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector MovementInput;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FRotator LastInputRotation;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxHP;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float CurrentHP;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PalyerMesh")
	class USkeletalMesh* PlayerMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SpringArmComp")
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="CameraComp")
	class UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LeftWeaponComp")
	class UStaticMeshComponent* LeftWeaponComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="RifhtWeaponComp")
	class UStaticMeshComponent* RightWeaponComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="NiagaraComp")
	class UNiagaraComponent* NiagaraComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BleedES")
	class UNiagaraSystem* BleedES;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SwordES")
	class UNiagaraSystem* SwordES;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DeadES")
	class UNiagaraSystem* DeadES;


	
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void MoveForward(float val);
	UFUNCTION(BlueprintCallable)
		void MoveRight(float val);
	UFUNCTION(BlueprintCallable)
		void LookUp(float val);
	UFUNCTION(BlueprintCallable)
		void LookRight(float val);
	UFUNCTION(BlueprintCallable)
		void DoJump();
	UFUNCTION(BlueprintCallable)
		void DoJumpCallBack();
	UFUNCTION(BlueprintCallable)
		void DoAttack();
	UFUNCTION(BlueprintCallable)
		void AttackSave();
	UFUNCTION(BlueprintCallable)
		void ComboReset();
	UFUNCTION(BlueprintCallable)
		void DoRoll();
	UFUNCTION(BlueprintCallable)
        void DoRollCallBack();
	UFUNCTION(BlueprintCallable)
		void DoHit();
	UFUNCTION(BlueprintCallable)
		void DieCallBack();
	UFUNCTION(BlueprintCallable)
		float IKLineTrace(FName FootName,float Distance);
	UFUNCTION(BlueprintCallable)
		void ChangeMeshSkin();
	UFUNCTION(BlueprintCallable)
		void ChangeChestSkin();
	UFUNCTION(BlueprintCallable)
		void ChangeShieldSkin();
	UFUNCTION(BlueprintCallable)
		void ChangeSwordSkin();
	UFUNCTION(BlueprintCallable)
		void ChangeAllSkin();
	UFUNCTION(BlueprintCallable)
		void ReliveCallBack();
	
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
