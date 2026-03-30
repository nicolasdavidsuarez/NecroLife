// Fill out your copyright notice in the Description page of Project Settings.


#include "NecroLifePlayerState.h"

#include "Components/AttributeComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/QuestComponent.h"


ANecroLifePlayerState::ANecroLifePlayerState()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	QuestComponent=CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));
}
