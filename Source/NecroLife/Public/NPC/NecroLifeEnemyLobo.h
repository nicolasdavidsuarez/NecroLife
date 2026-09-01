// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/NPC/NecroLifeEnemyBasic.h"
#include "Perception/PawnSensingComponent.h"
#include "NecroLifeEnemyLobo.generated.h"

UCLASS()
class NECROLIFE_API ANecroLifeEnemyLobo : public ANecroLifeEnemyBasic
{
	GENERATED_BODY()

public:
	ANecroLifeEnemyLobo();

	UPROPERTY(VisibleAnywhere, Category="AI")
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lobo|Movimiento")
	float IdleSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lobo|Movimiento")
	float ChaseSpeed = 450.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lobo|Vision")
	float SightRadius = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lobo|Vision")
	float VisionAngle = 60.f;

	// AttackRange heredado de ANecroLifeEnemyBasic (default 150)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lobo|Combate")
	float AttackCooldown = 1.2f;

	UPROPERTY(ReplicatedUsing=OnRep_IsChasing)
	bool bIsChasing = false;

	UFUNCTION()
	void OnRep_IsChasing();

	UFUNCTION(BlueprintImplementableEvent, Category="Lobo")
	void BP_OnStartChase();

	UFUNCTION(BlueprintImplementableEvent, Category="Lobo")
	void BP_OnAttack();

	UFUNCTION(BlueprintImplementableEvent, Category="Lobo")
	void BP_OnTakeDamage();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_OnTakeDamage();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	float PreviousHealth = 0.f;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnLoboHealthChanged(float Health, float HealthMax);
};
