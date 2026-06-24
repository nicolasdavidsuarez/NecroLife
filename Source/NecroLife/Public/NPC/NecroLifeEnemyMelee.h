// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/NPC/NecroLifeEnemyBasic.h"
#include "Perception/PawnSensingComponent.h"
#include "NecroLifeEnemyMelee.generated.h"

UENUM(BlueprintType)
enum class EEnemyMeleeState : uint8
{
	Patrolling  UMETA(DisplayName = "Patrolling"),
	Chasing     UMETA(DisplayName = "Chasing"),
	Attacking   UMETA(DisplayName = "Attacking"),
	Returning   UMETA(DisplayName = "Returning")
};

UCLASS()
class NECROLIFE_API ANecroLifeEnemyMelee : public ANecroLifeEnemyBasic
{
	GENERATED_BODY()

public:
	ANecroLifeEnemyMelee();

	UPROPERTY(VisibleAnywhere, Category="AI")
	UPawnSensingComponent* PawnSensing;

	// Estado replicado — el AnimBP lo lee directamente
	UPROPERTY(ReplicatedUsing=OnRep_MeleeState, BlueprintReadOnly, Category="AI")
	EEnemyMeleeState MeleeState = EEnemyMeleeState::Patrolling;

	UFUNCTION()
	void OnRep_MeleeState();

	// --- Movimiento ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Melee|Movimiento")
	float IdleSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Melee|Movimiento")
	float ChaseSpeed = 450.f;

	// --- Visión ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Melee|Vision")
	float SightRadius = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Melee|Vision")
	float VisionAngle = 180.f;

	// --- Combate ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Melee|Combate")
	float AttackRange = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Melee|Combate")
	float AttackCooldown = 1.5f;

	// Si el jugador se aleja más de esta distancia de la posición HOME, el enemigo vuelve
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Melee|Combate")
	float ChaseAbandonDistance = 2000.f;

	// --- Patrulla ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Melee|Patrulla")
	float PatrolRadius = 500.f;

	// Implementar en BP para audio/VFX
	UFUNCTION(BlueprintImplementableEvent, Category="Melee")
	void BP_OnStartChase();

	UFUNCTION(BlueprintImplementableEvent, Category="Melee")
	void BP_OnReturn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	FVector HomeLocation;
	FVector CurrentPatrolTarget;
	TWeakObjectPtr<APawn> TargetPlayer;
	float ChasePathTimer = 0.f;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	void SetState(EEnemyMeleeState NewState);
	void PickNewPatrolTarget();
	void TickPatrolling();
	void TickChasing();
	void TickAttacking();
	void TickReturning();
};
