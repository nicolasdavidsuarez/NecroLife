// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NecroLifePlayerState.generated.h"

/**
 * 
 */
class UAttributeComponent;
class UInventoryComponent;
class UQuestComponent;

UCLASS()
class NECROLIFE_API ANecroLifePlayerState : public APlayerState
{
	GENERATED_BODY()
	public:
	ANecroLifePlayerState();

	UAttributeComponent* GetAttributeComponent() const { return AttributeComponent; }
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	UQuestComponent* GetQuestComponent() const { return QuestComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UQuestComponent> QuestComponent;
};
