#pragma once

#include "CoreMinimal.h"
#include "FlowFieldGrid.h"
#include "Components/ActorComponent.h"
#include "FlowFieldComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (FlowField), meta = (BlueprintSpawnableComponent))
class FLOWFIELDCORE_API UFlowFieldComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UFlowFieldComponent();

    UPROPERTY(EditAnywhere, Category = "FlowField", meta = (ClampMin = "1.0"))
    float ChunkSize = 1000.f;

    UPROPERTY(EditAnywhere, Category = "FlowField", meta = (ClampMin = "1"))
    FIntPoint CellsPerChunk = { 32, 32 };

    UPROPERTY(EditAnywhere, Category = "FlowField|Debug") bool bDrawChunks = false;
    UPROPERTY(EditAnywhere, Category = "FlowField|Debug") bool bDrawArrows = false;
    UPROPERTY(EditAnywhere, Category = "FlowField|Debug", meta = (ClampMin = "1")) int32 DebugCellStride = 1;

    const FlowFieldGrid& GetGrid() const { return Grid; }

    virtual void TickComponent(float DT, ELevelTick, FActorComponentTickFunction*) override;
    virtual void BeginPlay() override;
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& E) override;
#endif

private:
    FlowFieldGrid Grid;
    void PushConfigToGrid();   // Grid.Init(ChunkSize, CellsPerChunk); Grid.Rebuild();
};