// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemDanio.generated.h"

/**
 * 
 */
UCLASS()
class NECROLIFE_API AItemDanio : public AItemBase
{
	GENERATED_BODY()
public:

	AItemDanio();

	FString mensaje = "Recibes danio";
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Props")
	int danio = 10;
};
