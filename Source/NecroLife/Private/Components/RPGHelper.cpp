// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/RPGHelper.h"
#include "Public/Components/UHealthComponent.h"
#include "Public/Components/AbilityComponent.h"
#include "Public/Components/AttributeComponent.h"
#include "Public/Components/InventoryComponent.h"
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
 bool URPGHelper::PickUpItem(AActor* Target, UItemData* Item)  //DatosItem  ItemData
{
	if (!Target) return false;
	if (UInventoryComponent* InventoryComponent = Target->FindComponentByClass<UInventoryComponent>())
	{
		//todo to do todo 
		InventoryComponent->PickUp(Item);
		return true;
	}
	return false;
}
