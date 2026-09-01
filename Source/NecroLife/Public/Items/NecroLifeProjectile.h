// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NecroLifeProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class NECROLIFE_API ANecroLifeProjectile : public AActor
{
	GENERATED_BODY()

public:
	ANecroLifeProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	float Damage = 15.f;

	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UProjectileMovementComponent* ProjectileMovement;

protected:
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DestroyProjectile();
};
