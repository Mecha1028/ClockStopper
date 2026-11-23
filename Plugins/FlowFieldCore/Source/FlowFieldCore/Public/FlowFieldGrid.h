#pragma once

#include "CoreMinimal.h"
#include "FlowFieldGrid.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FlowFieldCell {
	GENERATED_BODY()
	UPROPERTY()
	int32 CellCost = 1;
	UPROPERTY()
	int32 Integration = MAX_uint16;
	UPROPERTY()
	FVector2D FlowVector = FVector2D::ZeroVector;
	UPROPERTY()
	float Height = 0.f;
};

class UWorld;
class AActor;

class FLOWFIELDCORE_API FlowFieldGrid {
public:
	TArray<FlowFieldCell> Cells;

	int32 GridWidth = 0;
	int32 GridHeight = 0;
	float CellSize = 100.f;
	float WalkableFloorZ = 0.7;
	int32 MaxSlopeCost = 10;
	
	void GenerateGrid();
	bool GetNeighbourIntegration(int32 X, int32 Y, uint16& OutInt) const;
	void BuildIntegrationField(FIntPoint Goal);
	void BuildFlowField(FIntPoint Goal);

	bool IsInBounds(int32 X, int32 Y) const {
		return X >= 0 && X < GridWidth && Y >= 0 && Y < GridHeight;
	}

	FVector2D GetCellFlow(int32 X, int32 Y) const {
		if (!IsInBounds(X, Y)) {
			return FVector2D::ZeroVector;
		}
		const FlowFieldCell& Cell = Cells[ToIndex(X, Y)];
		if (Cell.CellCost == 255) {
			return FVector2D::ZeroVector;
		}
		return Cell.FlowVector;
	}

	int32 ToIndex(int32 X, int32 Y) const {
		return Y * GridWidth + X;
	}
	FVector CellToWorld(int32 X, int32 Y, const FVector& Origin) const;
	bool WorldToCell(const FVector& WorldPos, const FVector& Origin, int32& OutX, int32& OutY) const;

	void MarkObstacles(UWorld* World, const FVector& Origin, float TraceHeight, AActor* ActorToIgnore);
};