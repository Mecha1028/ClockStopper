#include "FlowFieldGrid.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

void FlowFieldGrid::GenerateGrid() {
	Cells.SetNum(GridWidth * GridHeight);
	for (FlowFieldCell& Cell : Cells) {
		Cell.CellCost = 1;
		Cell.Integration = MAX_uint16;
		Cell.FlowVector = FVector2D::ZeroVector;
		Cell.Height = 0.f;
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
	CollisionParams.bTraceComplex = true;
	CollisionParams.AddIgnoredActor(ActorToIgnore);
	for (int32 Y = 0; Y < GridHeight; Y++) {
		for (int32 X = 0; X < GridWidth; X++) {
			FVector Center = CellToWorld(X, Y, Origin);
			FVector Start = Center + FVector(0.f, 0.f, TraceHeight);
			FVector End = Center - FVector(0.f, 0.f, TraceHeight);

			TArray<FHitResult> Hits;
			World->LineTraceMultiByChannel(Hits, Start, End, ECC_WorldStatic, CollisionParams);
			FHitResult ValidHit;
			bool bFound = false;
			float BestDist = FLT_MAX;
			for (const FHitResult& Hit : Hits) {
				if (Hit.ImpactNormal.Z >= 0.f) {
					float Dist = FMath::Abs(Hit.ImpactPoint.Z - Origin.Z);
					if (Dist < BestDist) {
						BestDist = Dist;
						ValidHit = Hit;
						bFound = true;
					}
				}
			}
			
			if (bFound) {
				if (ValidHit.ImpactNormal.Z <= WalkableFloorZ) {
					Cells[ToIndex(X, Y)].CellCost = 255;
					Cells[ToIndex(X, Y)].Height = ValidHit.ImpactPoint.Z;
				}
				else {
					Cells[ToIndex(X, Y)].Height = ValidHit.ImpactPoint.Z;
					float Threshold = (ValidHit.ImpactNormal.Z - WalkableFloorZ) / (1.f - WalkableFloorZ);
					Cells[ToIndex(X, Y)].CellCost = FMath::RoundToInt(FMath::Lerp((float)MaxSlopeCost, 1.f, Threshold));
				}
			}
			else {
				Cells[ToIndex(X, Y)].CellCost = 255;
				Cells[ToIndex(X, Y)].Height = Origin.Z;
			}
		}
	}
}