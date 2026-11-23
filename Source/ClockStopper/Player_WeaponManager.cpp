#include "Player_WeaponManager.h"

UPlayer_WeaponManager::UPlayer_WeaponManager()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UPlayer_WeaponManager::BeginPlay()
{

	Super::BeginPlay();
	
}

void UPlayer_WeaponManager::GetWeaponData()
{



}

void UPlayer_WeaponManager::UseActiveWeapon()
{



}

void UPlayer_WeaponManager::SetActiveWeapon()
{



}

void UPlayer_WeaponManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

