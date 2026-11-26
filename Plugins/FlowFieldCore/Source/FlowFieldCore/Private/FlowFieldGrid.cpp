#include "FlowFieldGrid.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

static const FIntPoint NeighbourOffsets[8] = {
	FIntPoint(-1, -1),
	FIntPoint(0, -1),
	FIntPoint(1, -1),
	FIntPoint(-1, 0),
	FIntPoint(1, 0),
	FIntPoint(-1, 1),
	FIntPoint(0, 1),
	FIntPoint(1, 1)
};

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

void FlowFieldGrid::MarkObstacles(UWorld* World, const FVector& Origin, AActor* ActorToIgnore) {
	if (!World) {
		return;
	}
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = false;
	CollisionParams.AddIgnoredActor(ActorToIgnore);
	float FootprintHalf = CellSize * 0.5f;
	float BoxHalfHeight = AgentHeight * 0.5f;

	for (int32 Y = 0; Y < GridHeight; Y++) {
		for (int32 X = 0; X < GridWidth; X++) {
			FVector Center = CellToWorld(X, Y, Origin);
			FVector Start = FVector(Center.X, Center.Y, GridFloorZ + GroundProbeUp);
			FVector End = FVector(Center.X, Center.Y, GridFloorZ - GroundProbeDown);

			FHitResult Hit;
			bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, CollisionParams);
			
			const int32 Index = ToIndex(X, Y);
			if (bHit) {
				if (Hit.ImpactNormal.Z <= WalkableFloorZ) {
					Cells[Index].CellCost = 255;
					Cells[Index].Height = Hit.ImpactPoint.Z;
				}
				else {
					Cells[Index].Height = Hit.ImpactPoint.Z;
					float Threshold = (Hit.ImpactNormal.Z - WalkableFloorZ) / (1.f - WalkableFloorZ);
					Cells[Index].CellCost = FMath::RoundToInt(FMath::Lerp((float)MaxSlopeCost, 1.f, Threshold));
					//FVector BoxCenter(Center.X, Center.Y, Cells[Index].Height + BoxHalfHeight);
					FVector BoxCenter(Center.X, Center.Y, Cells[Index].Height + 1.f + BoxHalfHeight);
					FCollisionShape Box = FCollisionShape::MakeBox(FVector(FootprintHalf, FootprintHalf, BoxHalfHeight));
					bool bBlocked = World->OverlapBlockingTestByChannel(BoxCenter, FQuat::Identity, ECC_WorldStatic, Box, CollisionParams);
					if (bBlocked) {
						Cells[Index].CellCost = 255;
					}
				}
			}
			else {
				Cells[Index].CellCost = 255;
				Cells[Index].Height = GridFloorZ;
			}
		}
	}
}

void FlowFieldGrid::BuildIntegrationField(FIntPoint Goal) {
	for (FlowFieldCell& Cell : Cells) {
		Cell.Integration = MAX_uint16;
	}
	// Goal Validity
	if (!IsInBounds(Goal.X, Goal.Y)) {
		return;
	}
	if (Cells[ToIndex(Goal.X, Goal.Y)].CellCost == 255) {
		return;
	}

	TArray<TPair<uint16, int32>> Queue;
	const int32 GoalIndex = ToIndex(Goal.X, Goal.Y);
	Cells[GoalIndex].Integration = 0;
	Queue.HeapPush(TPair<uint16, int32>(0, GoalIndex));

	while (Queue.Num() > 0) {
		TPair<uint16, int32> Entry;
		Queue.HeapPop(Entry);
		const int32 CurrentIndex = Entry.Value;
		const int32 CurrentX = CurrentIndex % GridWidth;
		const int32 CurrentY = CurrentIndex / GridWidth;

		if (Cells[CurrentIndex].Integration < Entry.Key) {
			continue;
		}

		for (const FIntPoint& Offset : NeighbourOffsets) {
			int32 NeighbourX = CurrentX + Offset.X;
			int32 NeighbourY = CurrentY + Offset.Y;
			if (!IsInBounds(NeighbourX, NeighbourY)) {
				continue;
			}
			if (Cells[ToIndex(NeighbourX, NeighbourY)].CellCost == 255) {
				continue;
			}

			bool bDiagonal = (Offset.X != 0 && Offset.Y != 0);
			int32 StepCost = Cells[ToIndex(NeighbourX, NeighbourY)].CellCost * (bDiagonal ? 14 : 10);

			int32 NewInt = Cells[CurrentIndex].Integration + StepCost;

			int32 NeighbourIndex = ToIndex(NeighbourX, NeighbourY);

			if (NewInt < Cells[NeighbourIndex].Integration) {
				Cells[NeighbourIndex].Integration = (uint16)NewInt;
				Queue.HeapPush(TPair<uint16, int32>((uint16)NewInt, NeighbourIndex));
			}
		}
	}
}

bool FlowFieldGrid::GetNeighbourIntegration(int32 X, int32 Y, uint16& OutInt) const {
	if (!IsInBounds(X, Y)) {
		return false;
	}

	const FlowFieldCell& CurrentCell = Cells[ToIndex(X, Y)];
	if (CurrentCell.CellCost == 255) {
		return false;
	}

	OutInt = CurrentCell.Integration;
	return true;
}

void FlowFieldGrid::BuildFlowField(FIntPoint Goal) {
	for (int32 Y = 0; Y < GridHeight; Y++) {
		for (int32 X = 0; X < GridWidth; X++) {
			const FlowFieldCell& CurrentCell = Cells[ToIndex(X, Y)];
			if (CurrentCell.CellCost == 255) {
				continue;
			}
			if (CurrentCell.Integration == MAX_uint16) {
				continue;
			}
			if (X == Goal.X && Y == Goal.Y) {
				Cells[ToIndex(X, Y)].FlowVector = FVector2D::ZeroVector;
				continue;
			}

			uint16 IntEast = 0;
			uint16 IntWest = 0;
			uint16 IntNorth = 0;
			uint16 IntSouth = 0;
			bool bEast = GetNeighbourIntegration(X + 1, Y, IntEast);
			bool bWest = GetNeighbourIntegration(X - 1, Y, IntWest);
			bool bNorth = GetNeighbourIntegration(X, Y + 1, IntNorth);
			bool bSouth = GetNeighbourIntegration(X, Y - 1, IntSouth);

			int32 DirectionX = 0;
			int32 DirectionY = 0;

			if (bEast && bWest) {
				DirectionX = (int32)IntEast - (int32)IntWest;
			}
			else if (bEast) {
				DirectionX = (int32)IntEast - (int32)CurrentCell.Integration;
			}
			else if (bWest) {
				DirectionX = (int32)CurrentCell.Integration - (int32)IntWest;
			}
			if (bNorth && bSouth) {
				DirectionY = (int32)IntNorth - (int32)IntSouth;
			}
			else if (bNorth) {
				DirectionY = (int32)IntNorth - (int32)CurrentCell.Integration;
			}
			else if (bSouth) {
				DirectionY = (int32)CurrentCell.Integration - (int32)IntSouth;
			}

			if (DirectionX != 0 && DirectionY != 0) {
				int32 StepX = FMath::Sign(DirectionX);
				int32 StepY = FMath::Sign(DirectionY);
				int32 MidX = X - StepX;
				int32 MidY = Y - StepY;
				bool bMidXWalkable = IsCellWalkable(MidX, Y);
				bool bMidYWalkable = IsCellWalkable(X, MidY);

				if (!bMidXWalkable && !bMidYWalkable) {
					DirectionX = 0;
					DirectionY = 0;
				}
				else if (!bMidXWalkable && bMidYWalkable) {
					DirectionX = 0;
				}
				else if (bMidXWalkable && !bMidYWalkable) {
					DirectionY = 0;
				}
			}

			FVector2D Flow(-(double)DirectionX, -(double)DirectionY);
			Cells[ToIndex(X,Y)].FlowVector = Flow.GetSafeNormal();
		}
	}
}