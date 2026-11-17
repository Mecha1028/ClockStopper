#include "FlowFieldComponent.h"


UFlowFieldComponent::UFlowFieldComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
#if WITH_EDITORONLY_DATA
    bTickInEditor = true;
#endif
}

void UFlowFieldComponent::BeginPlay()
{
    Super::BeginPlay();
    PushConfigToGrid();
}

#if WITH_EDITOR
void UFlowFieldComponent::PostEditChangeProperty(FPropertyChangedEvent& E)
{
    Super::PostEditChangeProperty(E);
    const FName N = E.GetPropertyName();
    if (N == GET_MEMBER_NAME_CHECKED(UFlowFieldComponent, ChunkSize) ||
        N == GET_MEMBER_NAME_CHECKED(UFlowFieldComponent, CellsPerChunk))
    {
        PushConfigToGrid();
    }
}
#endif

void UFlowFieldComponent::PushConfigToGrid()
{
    Grid.Init(ChunkSize, CellsPerChunk);
    Grid.Rebuild();
}

// TickComponent: same DrawDebug* loop as before, but read from Grid.Chunks,
// Grid.ChunkSize, Grid.CellsPerChunk.