// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/NPC/NecroLifeEnemyRanged.h"
#include "Public/Items/NecroLifeProjectile.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

ANecroLifeEnemyRanged::ANecroLifeEnemyRanged()
{
	EsenciasAlMorir = 90.f;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bOnlySensePlayers = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ANecroLifeEnemyRanged::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->SightRadius = SightRadius;
	PawnSensing->SetPeripheralVisionAngle(VisionAngle);

	if (HasAuthority())
		PawnSensing->OnSeePawn.AddDynamic(this, &ANecroLifeEnemyRanged::OnSeePawn);
}

void ANecroLifeEnemyRanged::OnSeePawn(APawn* Pawn)
{
	if (bIsDead || !HasAuthority() || !Pawn || !Pawn->IsPlayerControlled()) return;

	AAIController* AIC = Cast<AAIController>(GetController());
	if (!AIC) return;

	UBlackboardComponent* BB = AIC->GetBlackboardComponent();
	if (!BB) return;

	BB->SetValueAsObject(FName("PlayerTarget"), Pawn);
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
