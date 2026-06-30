#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

#define CATEGORY_HEALTH Health

// 💡 meta=(BlueprintSpawnableComponent) — BP의 Add Component 목록에 이 컴포넌트가 뜨게 해주는 매크로. 컴포넌트엔 거의 항상 붙여요.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSWAVESHOOTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CATEGORY_HEALTH)
	int32 MaxHP = 100;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CATEGORY_HEALTH)
	int32 CurrentHP = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CATEGORY_HEALTH)
	bool bIsDead = false;
	
	UFUNCTION(BlueprintCallable, Category=CATEGORY_HEALTH)
	void ResetHealth();
	
	UFUNCTION(BlueprintCallable, Category=CATEGORY_HEALTH)
	void ApplyDamage(int32 Damage);
	
	UFUNCTION(BlueprintCallable, Category=CATEGORY_HEALTH)
	void MarkDead();
	
	// BlueprintPure — 실행 핀 없는 순수 함수 (값만 반환). GetHealthPercent처럼 읽기만 하는 함수에 씀.
	UFUNCTION(BlueprintPure, Category=CATEGORY_HEALTH)
	float GetHealthPercent() const;
		
};
