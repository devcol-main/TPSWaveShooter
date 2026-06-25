// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

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
	
	// Input
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* JumpAction;
	
	//TObjectPtr<class T>
	
protected:
	void InputActionMove(const struct FInputActionValue& Value);
	void InputActionLook(const struct FInputActionValue& Value);
	void InputActionJump(const struct FInputActionValue& Value);
	
		
	
	
};
