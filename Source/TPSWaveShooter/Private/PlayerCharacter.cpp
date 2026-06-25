// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"


#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/*
	⚠️ 자주 하는 실수: SpringArm과 Camera를 둘 다 bUsePawnControlRotation = true 로 두는 것.
회전이 두 배가 되거나 반드시 떨림이 생기는 건 아닙니다
다만 두 컴포넌트가 똑같이 Control Rotation을 직접 쓰게 돼서 설정이 중복됩니다 (Camera Lag, 커스텀 회전을 얹으면 그때 충돌 소지).
→ 그래서 일반적인 3인칭에선 SpringArm만 true(마우스 따라 회전)
Camera는 false(SpringArm의 회전을 그대로 상속)로 둡니다.
	 */

	//SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->bUsePawnControlRotation = true;

	//
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	// 몸은 컨트롤러 yaw에 맞춰 회전 (TPS 스타일)
	bUseControllerRotationYaw = true;
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

/*
// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction) EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InputActionMove);
		if (LookAction) EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InputActionLook);
		if (JumpAction) EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::InputActionJump);
	}
}

void APlayerCharacter::InputActionMove(const FInputActionValue& Value)
{
	const FVector2D MoveVec = Value.Get<FVector2D>();
	if (Controller == nullptr)
	{
		return;
	}

	// 카메라 yaw 방향 기준으로 forward/right 계산
	const FRotator Rotation = Controller->GetControlRotation(); // Controller 카메라랑 같은 것 
	
	const FRotator YawRotation(0, Rotation.Yaw, 0);  // pitch/roll 0, yaw만
	
	// 카메라 기준 방향
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MoveVec.Y);   // W/S → 앞뒤
	AddMovementInput(RightDirection, MoveVec.X);   // A/D → 좌우
}

void APlayerCharacter::InputActionLook(const FInputActionValue& Value)
{
	const FVector2D LookVec = Value.Get<FVector2D>();
	AddControllerYawInput(LookVec.X);
	AddControllerPitchInput(LookVec.Y);
}

void APlayerCharacter::InputActionJump(const FInputActionValue& Value)
{
	Jump();   // ACharacter가 기본 제공하는 점프 함수
	
}

