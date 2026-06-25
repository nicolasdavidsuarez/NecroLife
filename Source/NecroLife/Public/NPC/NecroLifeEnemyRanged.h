// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/NPC/NecroLifeEnemyBasic.h"
#include "Perception/PawnSensingComponent.h"
#include "NecroLifeEnemyRanged.generated.h"

class ANecroLifeProjectile;
class UAnimMontage;

UCLASS()
class NECROLIFE_API ANecroLifeEnemyRanged : public ANecroLifeEnemyBasic
{
	GENERATED_BODY()

public:
	ANecroLifeEnemyRanged();

	UPROPERTY(VisibleAnywhere, Category="AI")
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ranged|Vision")
	float SightRadius = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ranged|Vision")
	float VisionAngle = 180.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ranged")
	TSubclassOf<ANecroLifeProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ranged")
	float ShootRange = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ranged")
	float FleeDistance = 300.f;

	UPROPERTY(EditAnywhere, Category="Animations")
	UAnimMontage* ShootMontage;

	UFUNCTION(BlueprintCallable, Category="Ranged")
	void ShootProjectile(AActor* Target);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);
};
