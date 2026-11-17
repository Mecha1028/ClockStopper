#pragma once

#include "CoreMinimal.h"

/**
 *
 */

struct FChunkID {
	FIntPoint coordinates;
	
	FChunkID(FVector position, float chunkSize) {
		coordinates.X = FMath::FloorToInt(position.X / chunkSize);
		coordinates.Y = FMath::FloorToInt(position.Y / chunkSize);
	}

	bool operator == (const FChunkID & other) const {
		return coordinates == other.coordinates;
	}

	friend uint32 GetTypeHash(const FChunkID& ID) {
		return GetTypeHash(ID.coordinates);
	}
};

struct FChunk {
	FChunkID ID;
	FIntPoint Resolution;
	TArray<FVector> FlowField;
	
	int32 Index(int32 x, int32 y) const {
		return y * Resolution.X + x;
	}
};

class FLOWFIELDCORE_API FlowFieldGrid {
public:
	void Init(float InChunkSize, FIntPoint InCellsPerChunk);
	void Rebuild();
	FChunkID WorldToChunkID(const FVector& WorldPos) const;
	const FChunk* FindChunk(const FChunkID& ID) const;


	float ChunkSize = 1000.0f;
	FIntPoint CellsPerChunk = { 32,32 };

	TMap<FChunkID, FChunk> Chunks;

	float GetCellSize() const {
		return ChunkSize / CellsPerChunk.X;
	}
};
