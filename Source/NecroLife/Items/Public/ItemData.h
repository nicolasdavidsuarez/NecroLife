// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class NECROLIFE_API UItemData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item data")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item data")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item data")
	UTexture2D* Icon;
};
