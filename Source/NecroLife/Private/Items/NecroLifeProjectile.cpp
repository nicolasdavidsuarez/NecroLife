// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Items/NecroLifeProjectile.h"
#include "Public/Components/RPGHelper.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ANecroLifeProjectile::ANecroLifeProjectile()
{
	bReplicates = true;
	SetReplicateMovement(true);
	InitialLifeSpan = 4.f;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(20.f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ANecroLifeProjectile::OnSphereOverlap);
	SetRootComponent(CollisionSphere);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(CollisionSphere);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
}

void ANecroLifeProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	if (!OtherActor || OtherActor == GetInstigator()) return;

	URPGHelper::ApplyDamage(OtherActor, Damage);
	Multicast_DestroyProjectile();
}

void ANecroLifeProjectile::Multicast_DestroyProjectile_Implementation()
{
	Destroy();
}
