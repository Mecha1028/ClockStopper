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
	// Grid
	UPROPERTY(EditAnywhere, Category = "FlowField|Grid")
	int32 GridWidth = 64;
	UPROPERTY(EditAnywhere, Category = "FlowField|Grid")
	int32 GridHeight = 64;
	UPROPERTY(EditAnywhere, Category = "FlowField|Grid")
	float CellSize = 100.f;

	// Terrain
	UPROPERTY(EditAnywhere, Category = "FlowField|Terrain")
	float GridFloorZ = 0.f;
	UPROPERTY(EditAnywhere, Category = "FlowField|Terrain", meta = (UIMin = "0.0", UIMax = "1.0"))
	float WalkableFloorZ = 0.7;
	UPROPERTY(EditAnywhere, Category = "FlowField|Terrain")
	int32 MaxSlopeCost = 10;
	UPROPERTY(EditAnywhere, Category = "FlowField|Terrain")
	float GroundProbeUp = 100.f;
	UPROPERTY(EditAnywhere, Category = "FlowField|Terrain")
	float GroundProbeDown = 300.f;

	// Agent
	UPROPERTY(EditAnywhere, Category = "FlowField|Agent", meta = (ClampMin= "50.0", ClampMax = "500.0", UIMin = "50.0", UIMax = "400.0"))
	float AgentHeight = 180.f;

	// Goal
	UPROPERTY(EditAnywhere, Category = "FlowField|Goal")
	FIntPoint Goal = FIntPoint(32, 32); // 32 is temporary for testing
	UPROPERTY(EditAnywhere, Category = "FlowField|Goal")
	AActor* GoalActor;
	UPROPERTY(EditAnywhere, Category = "FlowField|Goal")
	float RebuildInterval = 0.2f;

	// Debug
	UPROPERTY(EditAnywhere, Category = "FlowField|Debug")
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