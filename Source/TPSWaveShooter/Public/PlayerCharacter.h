// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "WeaponDataAsset.h" // 이거 헤더 추가가 맞는지 모르겠다 + 추후 캐릭터 말고 다른 클래스로 이동 예정
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

#define CATEGORY_WEAPON Weapon




UCLASS()
class TPSWAVESHOOTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
public:
	// 3인칭 카메라 막대
	UPROPERTY(VisibleAnywhere, Category="Camera")
	class USpringArmComponent* SpringArmComponent;

	// 카메라 본체
	UPROPERTY(VisibleAnywhere, Category="Camera")
	class UCameraComponent* CameraComponent;
	
	// Input -> UPlayerInputConfigDataAsset
	/*
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* JumpAction;
	*/
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UPlayerInputConfigDataAsset* PlayerInputConfig;
	
	
	
protected:
	// === Input Action ===
	// Movements
	void InputActionMove(const struct FInputActionValue& Value);
	void InputActionLook(const struct FInputActionValue& Value);
	void InputActionJump(const struct FInputActionValue& Value);
	
	void InputActionFire(const struct FInputActionValue& Value);
	
	//Weapons
	// GrenadeLauncher MachineGun
	void InputActionSniperRifle(const struct FInputActionValue& Value);
	void InputActionGrenadeLauncher(const struct FInputActionValue& Value);
	void InputActionMachineGun(const struct FInputActionValue& Value);
	
	
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Setup")
	FName WeaponAttachSocketName = FName("hand_r"); 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Setup")
	float LineTraceRange = 5000.0f;
	
	
public:
	UPROPERTY(VisibleAnywhere, Category=CATEGORY_WEAPON)
	USkeletalMeshComponent* EquippedWeaponMesh;
	
public:
	UPROPERTY(EditDefaultsOnly, Category=CATEGORY_WEAPON)
	class UWeaponDataAsset* SniperRifleWeaponDataAsset;
	
	UPROPERTY(EditDefaultsOnly, Category=CATEGORY_WEAPON)
	class UWeaponDataAsset* GrenadeLauncherWeaponDataAsset;
	
	UPROPERTY(EditDefaultsOnly, Category=CATEGORY_WEAPON)
	class UWeaponDataAsset* MachineGunWeaponDataAsset;
	
	
	UPROPERTY(VisibleAnywhere, Category=CATEGORY_WEAPON)
	EWeaponType CurrentWeaponType = EWeaponType::SniperRifle;
	
protected:
	void TakeSniperRife();
	void TakeGrenadeLauncher();
	void TakeMachineGun();
	
	void EquipWeapon(class UWeaponDataAsset* WeaponDataAsset);
	
	void FireGun();

	
};
