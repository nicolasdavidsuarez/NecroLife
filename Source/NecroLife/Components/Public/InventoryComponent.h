// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Public/ItemData.h"
#include "InventoryComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPosionChange, int, CantPosiones);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowItem, const TArray<UItemData*>&, ItemsToShow);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NECROLIFE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	int HealthPosion=0;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void TakeHealthPosion(int Amount);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool UseHealtPosion();

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnPosionChange OnPosionChange;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnShowItem OnShowItem;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void PickUp(UItemData* Aitem);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<UItemData*> InventoryItems;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
