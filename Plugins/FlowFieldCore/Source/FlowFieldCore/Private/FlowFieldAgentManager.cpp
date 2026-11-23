#include "FlowFieldAgentManager.h"
#include "FlowFieldVolume.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"

AFlowFieldAgentManager::AFlowFieldAgentManager() {
	PrimaryActorTick.bCanEverTick = true;
	AgentInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("AgentInstances"));
	RootComponent = AgentInstances;
}

void AFlowFieldAgentManager::BeginPlay() {
	Super::BeginPlay();
	SpawnAgents();
}

void AFlowFieldAgentManager::SpawnAgents() {
	if (!Volume) {
		return;
	}
	if (Volume->Grid.Cells.Num() == 0) {
		Volume->Generate();
	}
	if (!AgentMesh) {
		return;
	}
	if (!AgentInstances) {
		return;
	}

	AgentInstances->ClearInstances();
	Agents.Empty();
	AgentInstances->SetStaticMesh(AgentMesh);
	Agents.Reserve(AgentCount);

	for (int32 i = 0; i < AgentCount; i++) {
		// Temporary Random Spawning, probably will change later
		int32 RandomX = FMath::RandRange(0, Volume->Grid.GridWidth - 1);
		int32 RandomY = FMath::RandRange(0, Volume->Grid.GridHeight - 1);
		const FlowFieldCell& Cell = Volume->Grid.Cells[Volume->Grid.ToIndex(RandomX, RandomY)];

		if (Cell.CellCost == 255) {
			continue; // this will not spawn full requested but that is fine for testing
		}
		FVector WorldPosition = Volume->Grid.CellToWorld(RandomX, RandomY, Volume->GetActorLocation());
		WorldPosition.Z = Cell.Height + 50.f;

		FAgentData NewAgent;
		NewAgent.Position = WorldPosition;
//		NewAgent.Speed = 300.f;
		NewAgent.bActive = true;
		Agents.Emplace(NewAgent);

		FTransform Transform(FRotator::ZeroRotator, WorldPosition, FVector(1.f));
		AgentInstances->AddInstance(Transform);
	}
}

void AFlowFieldAgentManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (!Volume) {
		return;
	}
	if (!AgentInstances) {
		return;
	}
	if (Agents.Num() == 0) {
		return;
	}

	const FVector Origin = Volume->GetActorLocation();

	for (int32 i = 0; i < Agents.Num(); i++) {
		if (!Agents[i].bActive) {
			continue;
		}

		FVector Delta = Agents[i].Position - Origin;
		float GridX = Delta.X / Volume->Grid.CellSize;
		float GridY = Delta.Y / Volume->Grid.CellSize;
		int32 CellX = FMath::FloorToInt(GridX);
		int32 CellY = FMath::FloorToInt(GridY);

		if (!Volume->Grid.IsInBounds(CellX,CellY)) {
			Agents[i].bActive = false;
			continue;
		}
		float FractionX = GridX - CellX;
		float FractionY = GridY - CellY;

		FVector2D BottomLeft = Volume->Grid.GetCellFlow(CellX, CellY);
		FVector2D BottomRight = Volume->Grid.GetCellFlow(CellX + 1, CellY);
		FVector2D TopLeft = Volume->Grid.GetCellFlow(CellX, CellY + 1);
		FVector2D TopRight = Volume->Grid.GetCellFlow(CellX + 1, CellY + 1);
		FVector2D Bottom = FMath::Lerp(BottomLeft, BottomRight, FractionX);
		FVector2D Top = FMath::Lerp(TopLeft, TopRight, FractionX);
		FVector2D Sampled = FMath::Lerp(Bottom, Top, FractionY);

		if (Sampled.IsNearlyZero()) {
			Agents[i].bActive = false;
			continue;
		}

		Agents[i].Position += FVector(Sampled.X, Sampled.Y, 0.f) * Agents[i].Speed * DeltaTime;
		Agents[i].Position.Z = Volume->Grid.Cells[Volume->Grid.ToIndex(CellX, CellY)].Height + 50.f;

		FTransform Transform(FRotator::ZeroRotator, Agents[i].Position, FVector(1.f));
		AgentInstances->UpdateInstanceTransform(i, Transform, true, false, true);
	}
	AgentInstances->MarkRenderStateDirty();
}