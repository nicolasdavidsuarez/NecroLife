// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/NPC/NecroLifeEnemyRanged.h"
#include "Public/Items/NecroLifeProjectile.h"

ANecroLifeEnemyRanged::ANecroLifeEnemyRanged()
{
}

void ANecroLifeEnemyRanged::ShootProjectile(AActor* Target)
{
	if (!HasAuthority() || !ProjectileClass || !Target) return;

	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 60.f;
	FRotator SpawnRotation = (Target->GetActorLocation() - SpawnLocation).Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.Owner = this;

	ANecroLifeProjectile* Projectile = GetWorld()->SpawnActor<ANecroLifeProjectile>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile)
	{
		Projectile->Damage = AttackDamage;
	}

	if (ShootMontage)
		PlayAnimMontage(ShootMontage);
}
