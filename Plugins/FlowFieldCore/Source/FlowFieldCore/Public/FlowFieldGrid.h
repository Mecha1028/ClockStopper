#pragma once

#include "CoreMinimal.h"

/**
 *
 */

struct FChunkID {

	FIntPoint coordinates;

	FChunkID(FVector position, float chunkSize) {
		coordinates.X = FMath::FloorToInt(position.X / chunkSize);
		coordinates.Y = FMath::FloorToInt(position.X / chunkSize);
	}

	bool operator == (const FChunkID & other) const {
		return coordinates == other.coordinates;
	}

	friend uint32 GetTypeHash(const FChunkID& ID) {
		return GetTypeHash(ID.coordinates);
	}
};

USTRUCT()
struct FChunk {
	GENERATED_BODY()

	UPROPERTY()
};

class FLOWFIELDCORE_API FlowFieldGrid
{

};
