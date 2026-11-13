#include "FlowFieldGrid.h"

void FlowFieldGrid::Init(int32 InWidth, int32 InHeight, float InCellSize, const FVector& InOrigin) {
	Width = FMath::Max(1, InWidth);
	Height = FMath::Max(1, InHeight);
	CellSize = FMath::Max(1.0f, InCellSize);
	Origin = InOrigin;
}