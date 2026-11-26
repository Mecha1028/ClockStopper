#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlowFieldVolume.h"
#include "FlowFieldAgentManager.generated.h"

/**
 * 
 */

USTRUCT()
struct FAgentData {
	GENERATED_BODY()
	FVector Position = FVector::ZeroVector;
	FVector FlowDirection = FVector::ZeroVector;
	float Speed = 300.f;
	bool bActive = true;
};

class UStaticMesh;
class UInstancedStaticMeshComponent;
class AFlowFieldVolume;

UCLASS(Blueprintable)
class FLOWFIELDCORE_API AFlowFieldAgentManager : public AActor {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Agents")
	AFlowFieldVolume* Volume;
	UPROPERTY(EditAnywhere, Category = "Agents")
	int32 AgentCount = 100; //Temporary Value
	UPROPERTY(EditAnywhere, Category = "Agents")
	UStaticMesh* AgentMesh;
	UPROPERTY(VisibleAnywhere, Category = "Agents")
	UInstancedStaticMeshComponent* AgentInstances;

	TArray<FAgentData> Agents;

	AFlowFieldAgentManager();
	void SpawnAgents();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
