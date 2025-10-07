// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemData.h"
#include "ItemRecolectable.generated.h"

/**
 * 
 */
UCLASS()
class NECROLIFE_API AItemRecolectable : public AItemBase
{
	GENERATED_BODY()
public:

	AItemRecolectable();	


	
	FString mensaje="Recolectaste";
	int cantidad;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Datos")
	UItemData* DatosDelItem;

	
};
