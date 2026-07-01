// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Monster/Monster.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "UObject/ConstructorHelpers.h"


// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 플레이어와 같은 마네킹 메시 재사용 -> 에셋을 하드 코딩으로 잡는건 실무 패턴은 아닙니다.
	// TODO: 추후 몬스터 에셋 변경
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(
		TEXT("/Game/External/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin"));
	
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 죽었으면: 한 번만 충돌/이동 끄고, 시체를 서서히 가라앉히다 제거
	// TODO: 추후 함수로 빼자.
	if (HealthComp && HealthComp->bIsDead)
	{
		if (!bDeathSequenceStarted)
		{
			bDeathSequenceStarted = true;
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			SetLifeSpan(4.0f);   // 4초 뒤 자동 Destroy
		}
		AddActorWorldOffset(FVector(0, 0, -CorpseSinkSpeed * DeltaTime));
	}

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

