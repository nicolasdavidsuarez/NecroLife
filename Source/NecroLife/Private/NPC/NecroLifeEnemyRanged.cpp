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

	// Disparar la animación en todos los clientes como evento confiable
	Multicast_TriggerAttackAnim();

	// Guardar el target y spawnar el proyectil con delay para sincronizar con la anim
	PendingTarget = Target;
	GetWorldTimerManager().SetTimer(SpawnDelayTimer, this, &ANecroLifeEnemyRanged::SpawnProjectileDelayed, ProjectileSpawnDelay, false);
}

void ANecroLifeEnemyRanged::SpawnProjectileDelayed()
{
	AActor* Target = PendingTarget.Get();
	if (!HasAuthority() || !Target || !ProjectileClass) return;

	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 60.f;
	FRotator SpawnRotation = (Target->GetActorLocation() - SpawnLocation).Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.Owner = this;

	ANecroLifeProjectile* Projectile = GetWorld()->SpawnActor<ANecroLifeProjectile>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile)
		Projectile->Damage = AttackDamage;
}

void ANecroLifeEnemyRanged::Multicast_TriggerAttackAnim_Implementation()
{
	BP_OnAttack();
	bIsEAttacking = true;
	GetWorldTimerManager().SetTimer(AttackAnimResetTimer, this, &ANecroLifeEnemyRanged::ResetAttackState, AttackAnimDuration, false);
}

void ANecroLifeEnemyRanged::ResetAttackState()
{
	bIsEAttacking = false;
}
