#include "FlowFieldGrid.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

void FlowFieldGrid::GenerateGrid() {
	Cells.SetNum(GridWidth * GridHeight);
	for (FlowFieldCell& Cell : Cells) {
		Cell.CellCost = 1;
		Cell.Integration = MAX_uint16;
		Cell.FlowVector = FVector2D::ZeroVector;
	}
}

FVector FlowFieldGrid::CellToWorld(int32 X, int32 Y, const FVector& Origin) const {
	FVector Offset((X + 0.5) * CellSize, (Y + 0.5) * CellSize, 0);
	return Origin + Offset;
}

bool FlowFieldGrid::WorldToCell(const FVector& WorldPos, const FVector& Origin, int32& OutX, int32& OutY) const {
	const FVector Delta = WorldPos - Origin;
	OutX = FMath::FloorToInt(Delta.X / CellSize);
	OutY = FMath::FloorToInt(Delta.Y / CellSize);

	bool BoundsCheck = OutX >= 0 && OutX < GridWidth && OutY >= 0 && OutY < GridHeight;
	return BoundsCheck;
}

void FlowFieldGrid::MarkObstacles(UWorld* World, const FVector& Origin, float TraceHeight, AActor* ActorToIgnore) {
	if (!World) {
		return;
	}

	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = false;
	CollisionParams.AddIgnoredActor(ActorToIgnore);
	for (int32 Y = 0; Y < GridHeight; Y++) {
		for (int32 X = 0; X < GridWidth; X++) {
			FHitResult Hit;
			FVector Center = CellToWorld(X, Y, Origin);
			FVector Start = Center + FVector(0.f, 0.f, TraceHeight);
			FVector End = Center - FVector(0.f, 0.f, TraceHeight);
			bool HitCheck = World->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, CollisionParams);
			if (HitCheck) {
				Cells[ToIndex(X, Y)].CellCost = 255;
			}
			else {
				Cells[ToIndex(X, Y)].CellCost = 1;
			}
		}
	}
}