#include "FlowFieldVolume.h"
#include "DrawDebugHelpers.h"

AFlowFieldVolume::AFlowFieldVolume() {
	PrimaryActorTick.bCanEverTick = true;
}

void AFlowFieldVolume::Generate() {
	Grid.GridWidth = GridWidth;
	Grid.GridHeight = GridHeight;
	Grid.CellSize = CellSize;
	Grid.WalkableFloorZ = WalkableFloorZ;
	Grid.MaxSlopeCost = MaxSlopeCost;
	Grid.GridFloorZ = GridFloorZ;
	Grid.AgentHeight = AgentHeight;
	Grid.GroundProbeUp = GroundProbeUp;
	Grid.GroundProbeDown = GroundProbeDown;
	Grid.GenerateGrid();
	Grid.MarkObstacles(GetWorld(), GetActorLocation(), this);
	Grid.BuildIntegrationField(Goal);
	Grid.BuildFlowField(Goal);
	DrawFlowArrows();
}

void AFlowFieldVolume::BeginPlay() {
	Super::BeginPlay();
	Generate();
}

void AFlowFieldVolume::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (!GoalActor) {
		return;
	}

	TimeSinceRebuild += DeltaTime;
	if (TimeSinceRebuild < RebuildInterval) {
		return;
	}
	TimeSinceRebuild = 0.f;
	int32 GoalX, GoalY;
	if (!Grid.WorldToCell(GoalActor->GetActorLocation(), GetActorLocation(), GoalX, GoalY)) {
		return;
	}
	SetGoal(FIntPoint(GoalX, GoalY));
}

void AFlowFieldVolume::SetGoal(FIntPoint NewGoal) {
	if (NewGoal == Goal) {
		return;
	}
	Goal = NewGoal;
	Grid.BuildIntegrationField(Goal);
	Grid.BuildFlowField(Goal);
	DrawFlowArrows();
}

void AFlowFieldVolume::DrawFlowArrows() {
	if (!bDebugDraw) {
		return;
	}
	if (!GetWorld() || !GetWorld()->IsGameWorld()) {
		return;
	}

	FlushPersistentDebugLines(GetWorld());

	for (int32 Y = 0; Y < Grid.GridHeight; Y++) {
		for (int32 X = 0; X < Grid.GridWidth; X++) {
			const FlowFieldCell& Cell = Grid.Cells[Grid.ToIndex(X, Y)];
			if (Cell.CellCost == 255) {
				continue;
			}
			if (Cell.FlowVector.IsNearlyZero()) {
				continue;
			}
			FVector Start = Grid.CellToWorld(X, Y, GetActorLocation());
			Start.Z = Cell.Height + 10.f;
			FVector End = Start + FVector(Cell.FlowVector.X, Cell.FlowVector.Y, 0.f) * CellSize * 0.7f;
			DrawDebugDirectionalArrow(GetWorld(), Start, End, CellSize * 0.3f, FColor::Cyan, true, -1.f, 1, 5.f);
		}
	}
}