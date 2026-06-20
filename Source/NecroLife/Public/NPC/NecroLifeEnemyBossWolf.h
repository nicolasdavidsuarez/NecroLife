// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/NPC/NecroLifeEnemyBasic.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SphereComponent.h"
#include "NecroLifeEnemyBossWolf.generated.h"

UENUM(BlueprintType)
enum class EBossWolfState : uint8
{
	Idle             UMETA(DisplayName = "Idle"),
	Howling          UMETA(DisplayName = "Howling"),
	WaitingForWolves UMETA(DisplayName = "Waiting For Wolves"),
	FreeDamage       UMETA(DisplayName = "Free Damage"),
	Charging         UMETA(DisplayName = "Charging"),
	Dashing          UMETA(DisplayName = "Dashing"),
	PostDash         UMETA(DisplayName = "Post Dash")
};

UCLASS()
class NECROLIFE_API ANecroLifeEnemyBossWolf : public ANecroLifeEnemyBasic
{
	GENERATED_BODY()

public:
	ANecroLifeEnemyBossWolf();

	// --- Percepción ---
	UPROPERTY(VisibleAnywhere, Category="Boss|AI")
	UPawnSensingComponent* PawnSensing;

	// Sphere separada para detectar jugadores durante el dash (no bloquea el movimiento)
	UPROPERTY(VisibleAnywhere, Category="Boss|Dash")
	USphereComponent* DashHitSphere;

	// --- Spawning de lobos ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Spawning")
	TSubclassOf<ANecroLifeEnemyBasic> WolfClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Spawning")
	int32 WolvesToSpawn = 4;

	// Offsets locales desde la posición del boss. Si está vacío usa una cruz de 200 unidades.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Spawning")
	TArray<FVector> WolfSpawnOffsets;

	// --- Tiempos (todos seteables en BP) ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Timing")
	float HowlDuration = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Timing")
	float FreeDamageTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Timing")
	float ChargeDuration = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Timing")
	float PostDashWaitTime = 3.f;

	// --- Dash ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Dash")
	float DashSpeed = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Dash")
	float DashDamage = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Dash")
	float DashHitRadius = 100.f;

	// Distancia al destino a la que se considera "llegado"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Dash")
	float DashArrivalThreshold = 150.f;

	// Actor vacío colocado en el nivel que marca el extremo del dash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Dash")
	AActor* DashTargetActor;

	// --- Visión ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Vision")
	float SightRadius = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss|Vision")
	float VisionAngle = 90.f;

	// --- Estado replicado (clientes reaccionan con BP events para animaciones) ---
	UPROPERTY(ReplicatedUsing=OnRep_BossState, BlueprintReadOnly, Category="Boss")
	EBossWolfState BossState = EBossWolfState::Idle;

	UFUNCTION()
	void OnRep_BossState();

	// --- Eventos Blueprint para disparar animaciones ---
	UFUNCTION(BlueprintImplementableEvent, Category="Boss|Animation")
	void BP_OnHowl();

	UFUNCTION(BlueprintImplementableEvent, Category="Boss|Animation")
	void BP_OnCharge();

	UFUNCTION(BlueprintImplementableEvent, Category="Boss|Animation")
	void BP_OnDash();

	UFUNCTION(BlueprintImplementableEvent, Category="Boss|Animation")
	void BP_OnFreeDamage();

	UFUNCTION(BlueprintImplementableEvent, Category="Boss|Animation")
	void BP_OnIdle();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	int32 AliveWolvesCount = 0;
	TSet<AActor*> DamagedActorsDuringDash;
	FVector BossStartLocation;
	bool bNextDashToTarget = true;
	FVector CurrentDashDestination;
	bool bActivated = false;

	FTimerHandle HowlTimerHandle;
	FTimerHandle FreeDamageTimerHandle;
	FTimerHandle ChargeTimerHandle;
	FTimerHandle PostDashTimerHandle;

	void SetBossState(EBossWolfState NewState);
	void SpawnWolves();
	void StartFreeDamagePhase();
	void StartCharging();
	void StartDashing();
	void OnDashArrived();
	void StartPostDash();

	UFUNCTION()
	void OnWolfDied(ANecroLifeEnemyBasic* Wolf);

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnDashHitSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
