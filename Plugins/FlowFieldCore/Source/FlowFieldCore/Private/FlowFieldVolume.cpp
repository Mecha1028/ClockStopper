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
	Grid.BuildIntegrationField(Goal);
	
//	FlushPersistentDebugLines(GetWorld());
}

void AFlowFieldVolume::BeginPlay() {
	Super::BeginPlay();
	Generate();
}