// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NecroLifeNpcBasic.h"
#include "../Components/InventoryComponent.h"
#include "NecroLifeForgeNpc.generated.h"

class FGemsItems;

UCLASS()
class NECROLIFE_API ANecroLifeForgeNpc : public ANecroLifeNpcBasic
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANecroLifeForgeNpc();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GemsToUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PercentOfUpgrade;

	TMap<FName, int32> CountGems;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnInteract_Implementation(AActor* Interactor) override;

	UFUNCTION(BlueprintCallable)
	TMap<FName, int32> GetPossibleUpgrades(const TArray<FDatosGema>& GemsItems);

	
	UFUNCTION(Server, Reliable)
	void Server_ForgeGems(const TArray<FGemsItems>& GemsItems);

	UFUNCTION()
	void ForgeGems(const TArray<FGemsItems>& GemsItems);
};
