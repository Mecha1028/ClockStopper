#include "FlowFieldVolume.h"
#include "DrawDebugHelpers.h"


void AFlowFieldVolume::Generate() {
	Grid.GridWidth = GridWidth;
	Grid.GridHeight = GridHeight;
	Grid.CellSize = CellSize;
	Grid.WalkableFloorZ = WalkableFloorZ;
	Grid.MaxSlopeCost = MaxSlopeCost;
	Grid.GenerateGrid();
	Grid.MarkObstacles(GetWorld(), GetActorLocation(), TraceHeight, this);
	
	FlushPersistentDebugLines(GetWorld());
	if (bDebugDraw) {
		for (int32 Y = 0; Y < GridHeight; Y++) {
			for (int32 X = 0; X < GridWidth; X++) {
				FVector Center = Grid.CellToWorld(X, Y, GetActorLocation());
				int32 Cost = Grid.Cells[Grid.ToIndex(X, Y)].CellCost;
				FLinearColor Colour;

				if (Cost >= 255) {
					Colour = FLinearColor::Red;
				}
				else if (Cost <= 1) {
					Colour = FLinearColor::Green;
				}
				else {
					Colour = FLinearColor::Yellow;
				}

				DrawDebugBox(GetWorld(), Center, FVector(CellSize * 0.5f, CellSize * 0.5f, 10.f), Colour.ToFColor(true), false, 3600.f, 0, 1.f);
			}
		}
	}
}

void AFlowFieldVolume::BeginPlay() {
	Super::BeginPlay();
	Generate();
}

void AFlowFieldVolume::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
#if WITH_EDITOR
	Generate();
#endif
}