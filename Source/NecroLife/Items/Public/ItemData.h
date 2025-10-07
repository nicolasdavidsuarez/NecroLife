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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Datos del Ítem")
	FText Nombre;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Datos del Ítem")
	FText Descripcion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Datos del Ítem")
	UTexture2D* Icono;
};
