// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "Public/Components/InventoryComponent.h"
#include "ItemGema.generated.h"

struct FDatosGema;

UCLASS()
class NECROLIFE_API AItemGema : public AItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemGema();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Gema")
    FDatosGema GemaData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Called every frame
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void Tick(float DeltaTime) override;
};
