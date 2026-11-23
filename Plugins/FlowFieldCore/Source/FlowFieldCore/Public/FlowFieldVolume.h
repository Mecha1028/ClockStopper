#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlowFieldGrid.h"
#include "FlowFieldVolume.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FLOWFIELDCORE_API AFlowFieldVolume : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "FlowField")
	int32 GridWidth = 64;
	UPROPERTY(EditAnywhere, Category = "FlowField")
	int32 GridHeight = 64;
	UPROPERTY(EditAnywhere, Category = "FlowField")
	float CellSize = 100.f;
	UPROPERTY(EditAnywhere, Category = "FlowField", meta = (UIMin = "0.0", UIMax = "1.0"))
	float WalkableFloorZ = 0.7;
	UPROPERTY(EditAnywhere, Category = "FlowField")
	int32 MaxSlopeCost = 10;
	UPROPERTY(EditAnywhere, Category = "FlowField")
	float TraceHeight = 500.f;
	UPROPERTY(EditAnywhere, Category = "FlowField")
	FIntPoint Goal = FIntPoint(32, 32); // 32 is temporary for testing
	UPROPERTY(EditAnywhere, Category = "FlowField")
	AActor* GoalActor;
	UPROPERTY(EditAnywhere, Category = "FlowField")
	float RebuildInterval = 0.2f;
	UPROPERTY(EditAnywhere, Category = "FlowField")
	bool bDebugDraw = true;

	FlowFieldGrid Grid;

	UFUNCTION(BlueprintCallable)
	void Generate();
	UFUNCTION(BlueprintCallable)
	void SetGoal(FIntPoint NewGoal);

	AFlowFieldVolume();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	float TimeSinceRebuild = 0.f;
	void DrawFlowArrows();
};