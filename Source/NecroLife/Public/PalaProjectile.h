#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PalaProjectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class NECROLIFE_API APalaProjectile : public AActor
{
	GENERATED_BODY()
    
public:	
	APalaProjectile();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	
	// --- Componentes ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USkeletalMeshComponent* PalaMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;
	

	// --- Variables de Combate ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DamageAmount = 25.0f;

	// --- Comportamiento de Vuelo (Onda Senoidal) ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|SineWave")
	bool bIsSinusoidal = false; // Checkbox para activar la S

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|SineWave")
	float SineAmplitude = 800.0f; // Qué tan "ancha" es la S

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|SineWave")
	float SineFrequency = 10.0f; // Qué tan "rápido" zigzaguea
	
	
	// Memoria de enemigos golpeados (para atravesarlos sin hacerles daño x60 veces)
	UPROPERTY()
	TArray<AActor*> DamagedActors;

	// --- Funciones ---
	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Memoria para saber hacia dónde se disparó originalmente
	FVector InitialForwardDir;
	FVector InitialRightDir;
	
};