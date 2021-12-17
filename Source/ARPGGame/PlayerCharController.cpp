// Created By Tong

#include "PlayerCharController.h"
#include "PlayerChar.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceConstant.h"

APlayerCharController::APlayerCharController()
{
	//径向模糊效果加载
	InputComponent=CreateDefaultSubobject<UInputComponent>(TEXT("inputs"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> CameraScreenEffect(TEXT("MaterialInstanceConstant'/Game/Material/PostProcessInstance.PostProcessInstance'"));
	post=CameraScreenEffect.Object;
}

//输入绑定
void APlayerCharController::SetupInputComponent()
{
	InputComponent->BindAxis("MoveForward",this, &APlayerCharController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharController::MoveRight);
	InputComponent->BindAxis("LookUp",this,&APlayerCharController::LookUp);
	InputComponent->BindAxis("LookRight",this,&APlayerCharController::LookRight);
	InputComponent->BindAction("Attack",IE_Pressed,this,&APlayerCharController::DoAttack);
	InputComponent->BindAction("Roll",IE_Pressed,this,&APlayerCharController::DoRoll);
	InputComponent->BindAction("Jump",IE_Pressed,this,&APlayerCharController::DoJump);
	InputComponent->BindAction("Run",IE_Pressed,this,&APlayerCharController::DoRun);
	InputComponent->BindAction("Run",IE_Released,this,&APlayerCharController::DoWalk);
	InputComponent->BindAction("ChangeMeshSkin",IE_Pressed,this,&APlayerCharController::DoChangeMeshSkin);
	InputComponent->BindAction("ChangeChestSkin",IE_Pressed,this,&APlayerCharController::DoChangeChestSkin);
	InputComponent->BindAction("ChangeShieldSkin",IE_Pressed,this,&APlayerCharController::DoChangeShieldSkin);
	InputComponent->BindAction("ChangeSwordSkin",IE_Pressed,this,&APlayerCharController::DoChangeSwordSkin);
	InputComponent->BindAction("ChangeAllSkin",IE_Pressed,this,&APlayerCharController::DoChangeAllSkin);
}

//移动
void APlayerCharController::MoveForward(float val)
{
	APlayerChar* PlayerChar=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	PlayerChar->MoveForward(val);
}

void APlayerCharController::MoveRight(float val)
{
	APlayerChar* PlayerChar=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	PlayerChar->MoveRight(val);
}

//视角
void APlayerCharController::LookUp(float val)
{
	AddPitchInput(val);
}

void APlayerCharController::LookRight(float val)
{
	AddYawInput(val);
}

//其他动作
void APlayerCharController::DoJump()
{
	APlayerChar* PlayerChar=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	PlayerChar->DoJump();
}


void APlayerCharController::DoAttack()
{
	APlayerChar* PlayerChar=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	PlayerChar->DoAttack();
}


void APlayerCharController::DoRoll()
{
	APlayerChar* PlayerChar=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	PlayerChar->DoRoll();
}

void APlayerCharController::DoRun()
{
	APlayerChar* PlayerChar=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	PlayerChar->GetCharacterMovement()->MaxWalkSpeed+=500.f;
	PlayerChar->CameraComp->AddOrUpdateBlendable(post,0.02f);
}

void APlayerCharController::DoWalk()
{
	APlayerChar* PlayerChar=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	PlayerChar->GetCharacterMovement()->MaxWalkSpeed-=500.f;
	PlayerChar->CameraComp->RemoveBlendable(post);
}

void APlayerCharController::DoChangeMeshSkin()
{
	APlayerChar* PlayerChar=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	PlayerChar->ChangeMeshSkin();
}

void APlayerCharController::DoChangeChestSkin()
{
	APlayerChar* PlayerChar=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	PlayerChar->ChangeChestSkin();
}

void APlayerCharController::DoChangeShieldSkin()
{
	APlayerChar* PlayerChar=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	PlayerChar->ChangeShieldSkin();
}

void APlayerCharController::DoChangeSwordSkin()
{
	APlayerChar* PlayerChar=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	PlayerChar->ChangeSwordSkin();
}

void APlayerCharController::DoChangeAllSkin()
{
	APlayerChar* PlayerChar=Cast<APlayerChar>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	PlayerChar->ChangeAllSkin();
}

