#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player_InventoryManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLOCKSTOPPER_API UPlayer_InventoryManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayer_InventoryManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Inventory;

	UFUNCTION(BlueprintCallable)
	void AddToInventory();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
