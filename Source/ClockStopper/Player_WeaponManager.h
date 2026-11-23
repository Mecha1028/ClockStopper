#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player_WeaponManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLOCKSTOPPER_API UPlayer_WeaponManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayer_WeaponManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* ActiveWeapon;

	void GetWeaponData();

	UFUNCTION(BlueprintCallable)
	void UseActiveWeapon();

	UFUNCTION(BlueprintCallable)
	void SetActiveWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
