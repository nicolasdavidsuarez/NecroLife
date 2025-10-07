// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Public/RPGHelper.h"
#include "Components/Public/UHealthComponent.h"
#include "Components/Public/AbilityComponent.h"
#include "Components/Public/AttributeComponent.h"
#include "Components/Public/InventoryComponent.h"
////////////////////////
/////HealthComponent
/////////////////////
bool URPGHelper::ApplyDamage(AActor* Target, float Amount)
{
	if (!Target) return false;

	if (UUHealthComponent* HealthComp = Target->FindComponentByClass<UUHealthComponent>())
	{
		HealthComp->TakeDamage(Amount);
		return true;
	}

	return false;
}

bool URPGHelper::ApplyHealing(AActor* Target, float Amount)
{
	if (!Target) return false;

	if (UUHealthComponent* HealthComp = Target->FindComponentByClass<UUHealthComponent>())
	{
		HealthComp->ApplyHealing(Amount);
		return true;
	}
	return false;
}
///////////////////
///AttributeComponent
////////////////////////
bool URPGHelper::TakeXP(AActor* Target, float Amount)
{
	if (!Target) return false;

	if (UAttributeComponent* AttributeComponent = Target->FindComponentByClass<UAttributeComponent>())
	{
		AttributeComponent->TakeXP(Amount);
		return true;
	}
	return false;
}
bool URPGHelper::TakePosion(AActor* Target)
{
	if (!Target) return false;

	if (UInventoryComponent* InventoryComponent = Target->FindComponentByClass<UInventoryComponent>())
	{
		InventoryComponent->TakeHealthPosion(1);
		return true;
	}
	return false;
}

///////////////////////////////////////////////////
///Inventory Component
////////////////////////////////////////////////////
bool URPGHelper::PickUpItem(AActor* Target, AActor* Item)  //DatosItem  ItemData
{
	if (!Target) return false;
	if (UInventoryComponent* InventoryComponent = Target->FindComponentByClass<UInventoryComponent>())
	{
		//todo to do todo 
	//	InventoryComponent->PickUp();
		return true;
	}
	return false;
}
