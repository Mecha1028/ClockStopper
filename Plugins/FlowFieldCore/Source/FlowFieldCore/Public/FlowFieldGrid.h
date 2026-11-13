#pragma once

#include "CoreMinimal.h"

/**
 * A 2D uniform grid that stores walkability, integration cost,
 * and a per-cell flow vector.
 */
class FLOWFIELDCORE_API FlowFieldGrid
{
public:
	// Initialization
	void Init(int32 InWidth, int32 InHeight, float InCellSize, const FVector& InOrigin);
	void Reset();

	// Coordinate Conversions (Local and Grid)
	int32 GridToIndex(int32 X, int32 Y) const {
		return Y * Width + X;
	}
	bool WorldToGrid(const FVector& WorldPosition, int32& OutX, int32& OutY) const;
	FVector GridToWorld(int32 X, int32 Y) const;

	// Blocked Checks
	void SetBlocked(int32 X, int32 Y, bool bBlocked);
	bool IsBlocked(int32 X, int32 Y) const;

	// Query Flow Field
	// Returns zero vector if outside the grid or if the cell is blocked
	FVector SampleDirection(const FVector& WorldPosition) const;

	// Accessors
	int32 GetWidth() const {
		return Width;
	}
	int32 GetHeight() const {
		return Height;
	}
	float GetCellSize() const {
		return CellSize;
	}
	const FVector& GetOrigin() const {
		return Origin;
	}
};
