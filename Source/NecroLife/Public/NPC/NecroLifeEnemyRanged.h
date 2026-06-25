// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/NPC/NecroLifeEnemyBasic.h"
#include "Perception/PawnSensingComponent.h"
#include "NecroLifeEnemyRanged.generated.h"

class ANecroLifeProjectile;

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

	// Delay desde que empieza la animación hasta que spawnea el proyectil
	UPROPERTY(EditAnywhere, Category="Animations")
	float ProjectileSpawnDelay = 0.3f;

	// Cuánto tiempo dura bIsEAttacking en true (largo de la anim de ataque)
	UPROPERTY(EditAnywhere, Category="Animations")
	float AttackAnimDuration = 0.8f;

	UFUNCTION(BlueprintCallable, Category="Ranged")
	void ShootProjectile(AActor* Target);

	// Implementar en BP_EnemyRanged para reproducir la animación de ataque (Play Anim Montage)
	UFUNCTION(BlueprintImplementableEvent, Category="Animations")
	void BP_OnAttack();

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle SpawnDelayTimer;
	FTimerHandle AttackAnimResetTimer;
	TWeakObjectPtr<AActor> PendingTarget;

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_TriggerAttackAnim();

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	void SpawnProjectileDelayed();
	void ResetAttackState();
};
