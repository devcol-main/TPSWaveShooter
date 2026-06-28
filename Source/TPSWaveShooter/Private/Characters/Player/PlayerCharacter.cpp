// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/Player/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputActionValue.h"
#include "Input/PlayerInputConfigDataAsset.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"

//#include "Weapons/WeaponDataAsset.h" PlayerCharacter.h 에서 선언함.
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
	
	// 무기 메쉬
	EquippedWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquippedWeaponMesh"));
	
	//EquippedWeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_r"));
	EquippedWeaponMesh->SetupAttachment(GetMesh(), WeaponAttachSocketName);
	
	EquippedWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EquippedWeaponMesh->SetVisibility(false);
	
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
			/*
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
			*/
			
			if (PlayerInputConfig && PlayerInputConfig->InputMappingContext)
			{
				Subsystem->AddMappingContext(PlayerInputConfig->InputMappingContext, 0);
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

	if (!PlayerInputConfig) return;
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		/*
		if (MoveAction) EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InputActionMove);
		if (LookAction) EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InputActionLook);
		if (JumpAction) EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::InputActionJump);
		*/
		
		// Movements
		if (PlayerInputConfig->MoveAction)
			EIC->BindAction(PlayerInputConfig->MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InputActionMove);
		if (PlayerInputConfig->LookAction)
			EIC->BindAction(PlayerInputConfig->LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InputActionLook);
		if (PlayerInputConfig->JumpAction)
			EIC->BindAction(PlayerInputConfig->JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::InputActionJump);
		
		//
		if (PlayerInputConfig->FireAction)
			EIC->BindAction(PlayerInputConfig->FireAction, ETriggerEvent::Started, this, &APlayerCharacter::InputActionFire);
		
		// Weapon Actions
		if (PlayerInputConfig->SniperRifleAction)
			EIC->BindAction(PlayerInputConfig->SniperRifleAction, ETriggerEvent::Started, this, &APlayerCharacter::InputActionSniperRifle);
		if (PlayerInputConfig->GrenadeLauncherAction)
			EIC->BindAction(PlayerInputConfig->GrenadeLauncherAction, ETriggerEvent::Started, this, &APlayerCharacter::InputActionGrenadeLauncher);
		if (PlayerInputConfig->MachineGunAction)
			EIC->BindAction(PlayerInputConfig->MachineGunAction, ETriggerEvent::Started, this, &APlayerCharacter::InputActionMachineGun);
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

// TODO: 추후 점프 관련 로직 설정하자
void APlayerCharacter::InputActionJump(const FInputActionValue& Value)
{
	Jump();   // ACharacter가 기본 제공하는 점프 함수
	
}

void APlayerCharacter::InputActionFire(const struct FInputActionValue& Value)
{
	FireGun();
}

void APlayerCharacter::InputActionSniperRifle(const struct FInputActionValue& Value)
{
	TakeSniperRife();
}

void APlayerCharacter::InputActionGrenadeLauncher(const struct FInputActionValue& Value)
{
	TakeGrenadeLauncher();
}

void APlayerCharacter::InputActionMachineGun(const struct FInputActionValue& Value)
{
	TakeMachineGun();
}


void APlayerCharacter::TakeSniperRife()
{
	EquipWeapon(SniperRifleWeaponDataAsset);
}

void APlayerCharacter::TakeGrenadeLauncher()
{
	EquipWeapon(GrenadeLauncherWeaponDataAsset);
}

void APlayerCharacter::TakeMachineGun()
{
	EquipWeapon(MachineGunWeaponDataAsset);
}

void APlayerCharacter::EquipWeapon(class UWeaponDataAsset* WeaponDataAsset)
{
	if (!WeaponDataAsset) return;
	
	CurrentWeaponType = WeaponDataAsset->WeaponType;
	
	if (WeaponDataAsset->Mesh)
	{
		EquippedWeaponMesh->SetSkeletalMesh(WeaponDataAsset->Mesh);
	}
	
	EquippedWeaponMesh->SetRelativeTransform(WeaponDataAsset->AttachTransform);
	EquippedWeaponMesh->SetVisibility(true);
}

// TODO: 현재 카메라에서만 나가는데 추후, 총의 위치에서도 나가게 해서 확인 하는 방식 추가하자
// TODO: 해당 코드는 거의 반복해서 쓰여진다고하니 추후 완성되면 정리 + Template 프로젝트로 올리자
void APlayerCharacter::FireGun()
{
	// TODO: 우선은 저격총만인데, 추후 다른 총들도 넣어야함
	if (CurrentWeaponType != EWeaponType::SniperRifle) return;
	
	if (!SniperRifleWeaponDataAsset) return;
	
	// 카메라 위치에서 전방 위치까지 라인 트레이스
	
	FVector LineTraceStart = CameraComponent->GetComponentLocation();
	FVector LineTraceEnd = LineTraceStart + CameraComponent->GetForwardVector() * LineTraceRange;
	
	FHitResult LineTraceHitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this); // 자신 무시
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		LineTraceHitResult, LineTraceStart, LineTraceEnd, ECC_Visibility, TraceParams);
	
	// 디버그 시각화
	DrawDebugLine(GetWorld(), LineTraceStart, LineTraceEnd, FColor::Red, false, 0.5f,0, 2.0f);
	
	if (bHit && LineTraceHitResult.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("LineTrace Hit Actor: %s | Damage: %d"), 
			*LineTraceHitResult.GetActor()->GetName(), SniperRifleWeaponDataAsset->Damage);
		
		DrawDebugSphere(GetWorld(), LineTraceHitResult.ImpactNormal, 15.0f, 12, FColor::Yellow, false, 0.5f);
	}
	
}

