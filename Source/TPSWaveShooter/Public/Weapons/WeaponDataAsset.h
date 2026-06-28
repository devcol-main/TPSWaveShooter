#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

#define CATEGORY_WEAPON Weapon

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	SniperRifle		UMETA(DisplayName = "Sniper Rifle"),
	GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),
	MachineGun		UMETA(DisplayName = "Machine Gun"),
};

UCLASS()
class TPSWAVESHOOTER_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

	
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=CATEGORY_WEAPON)
	EWeaponType WeaponType = EWeaponType::SniperRifle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=CATEGORY_WEAPON)
	USkeletalMesh* Mesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=CATEGORY_WEAPON)
	int32 Damage = 30;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=CATEGORY_WEAPON)
	FTransform AttachTransform = FTransform::Identity;
};
