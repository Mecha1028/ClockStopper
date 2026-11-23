#include "Player_InventoryManager.h"

UPlayer_InventoryManager::UPlayer_InventoryManager()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void UPlayer_InventoryManager::BeginPlay()
{

	Super::BeginPlay();
	
}

void UPlayer_InventoryManager::AddToInventory()
{



}


void UPlayer_InventoryManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

