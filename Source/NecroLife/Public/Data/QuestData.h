// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestData.generated.h"

/**
 * 
 */
UCLASS()
class NECROLIFE_API UQuestData : public UDataAsset
{
	GENERATED_BODY()
	public:
	// Nombre o de la Quest
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="General")
	FName QuestName;
	// Descripcion de la Quest
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="General")
	FName Description;
	
};
