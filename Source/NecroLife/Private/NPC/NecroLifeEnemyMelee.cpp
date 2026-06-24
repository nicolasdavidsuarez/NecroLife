// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/NPC/NecroLifeEnemyMelee.h"
#include "Net/UnrealNetwork.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"

ANecroLifeEnemyMelee::ANecroLifeEnemyMelee()
{
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bOnlySensePlayers = true;

	// Necesario para que PawnSensing funcione
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EsenciasAlMorir = 50.f;

	// Bug 3: rango suficiente para que no requiera estar encima del jugador
	AttackRange = 200.f;

	// Bug 4: radio más grande y sin depender de un socket que puede no existir
	AttackSocketName = FName("hand_r");
	AttackRadius = 80.f;
}

void ANecroLifeEnemyMelee::BeginPlay()
{
	Super::BeginPlay();

	HomeLocation = GetActorLocation();
	PawnSensing->SightRadius = SightRadius;
	PawnSensing->SetPeripheralVisionAngle(VisionAngle);
	GetCharacterMovement()->MaxWalkSpeed = IdleSpeed;

	if (HasAuthority())
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ANecroLifeEnemyMelee::OnSeePawn);
		PickNewPatrolTarget();
	}
}

void ANecroLifeEnemyMelee::OnSeePawn(APawn* Pawn)
{
	if (bIsDead || !HasAuthority() || !Pawn || !Pawn->IsPlayerControlled()) return;
	if (MeleeState == EEnemyMeleeState::Chasing || MeleeState == EEnemyMeleeState::Attacking) return;

	TargetPlayer = Pawn;
	SetState(EEnemyMeleeState::Chasing);
}

void ANecroLifeEnemyMelee::SetState(EEnemyMeleeState NewState)
{
	if (MeleeState == NewState) return;

	MeleeState = NewState;
	OnRep_MeleeState(); // El servidor también ejecuta localmente (patrón del proyecto)

	AAIController* AIC = Cast<AAIController>(GetController());

	switch (NewState)
	{
	case EEnemyMeleeState::Chasing:
		GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
		ChasePathTimer = 0.f; // Forzar request de path inmediato
		BP_OnStartChase();
		break;

	case EEnemyMeleeState::Patrolling:
		GetCharacterMovement()->MaxWalkSpeed = IdleSpeed;
		PickNewPatrolTarget();
		break;

	case EEnemyMeleeState::Returning:
		GetCharacterMovement()->MaxWalkSpeed = IdleSpeed;
		BP_OnReturn();
		if (AIC) AIC->MoveToLocation(HomeLocation, 50.f);
		break;

	case EEnemyMeleeState::Attacking:
		if (AIC) AIC->StopMovement();
		break;
	}
}

void ANecroLifeEnemyMelee::PickNewPatrolTarget()
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return;

	FNavLocation NavLoc;
	if (NavSys->GetRandomReachablePointInRadius(HomeLocation, PatrolRadius, NavLoc))
		CurrentPatrolTarget = NavLoc.Location;
	else
		CurrentPatrolTarget = HomeLocation;

	if (AAIController* AIC = Cast<AAIController>(GetController()))
		AIC->MoveToLocation(CurrentPatrolTarget, 50.f);
}

void ANecroLifeEnemyMelee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority() || bIsDead) return;

	ChasePathTimer = FMath::Max(0.f, ChasePathTimer - DeltaTime);

	switch (MeleeState)
	{
	case EEnemyMeleeState::Patrolling: TickPatrolling(); break;
	case EEnemyMeleeState::Chasing:    TickChasing();    break;
	case EEnemyMeleeState::Attacking:  TickAttacking();  break;
	case EEnemyMeleeState::Returning:  TickReturning();  break;
	}
}

void ANecroLifeEnemyMelee::TickPatrolling()
{
	if (FVector::Dist2D(GetActorLocation(), CurrentPatrolTarget) < 100.f)
		PickNewPatrolTarget();
}

void ANecroLifeEnemyMelee::TickChasing()
{
	APawn* Target = TargetPlayer.Get();
	if (!IsValid(Target))
	{
		SetState(EEnemyMeleeState::Returning);
		return;
	}

	// Jugador demasiado lejos de HOME → volver
	if (FVector::Dist2D(Target->GetActorLocation(), HomeLocation) > ChaseAbandonDistance)
	{
		TargetPlayer.Reset();
		SetState(EEnemyMeleeState::Returning);
		return;
	}

	// En rango de ataque → atacar
	if (FVector::Dist2D(GetActorLocation(), Target->GetActorLocation()) <= AttackRange)
	{
		SetState(EEnemyMeleeState::Attacking);
		return;
	}

	// Actualizar path cada 0.25s para seguir al jugador en movimiento
	if (ChasePathTimer <= 0.f)
	{
		if (AAIController* AIC = Cast<AAIController>(GetController()))
			AIC->MoveToActor(Target, AttackRange * 0.9f);
		ChasePathTimer = 0.25f;
	}
}

void ANecroLifeEnemyMelee::TickAttacking()
{
	APawn* Target = TargetPlayer.Get();
	if (!IsValid(Target))
	{
		SetState(EEnemyMeleeState::Returning);
		return;
	}

	float DistToPlayer = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());

	// 2x para no oscilar entre Attacking y Chasing con pequeños movimientos
	if (DistToPlayer > AttackRange * 2.0f)
	{
		float DistFromHome = FVector::Dist2D(Target->GetActorLocation(), HomeLocation);
		SetState(DistFromHome > ChaseAbandonDistance ? EEnemyMeleeState::Returning : EEnemyMeleeState::Chasing);
		return;
	}

	// Girar hacia el jugador mientras ataca
	FRotator LookAt = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation();
	LookAt.Pitch = 0.f;
	LookAt.Roll  = 0.f;
	SetActorRotation(LookAt);

	// El daño lo dispara el AnimNotify (ExecuteMeleeAttack) — no se llama desde acá
}

void ANecroLifeEnemyMelee::TickReturning()
{
	if (FVector::Dist2D(GetActorLocation(), HomeLocation) < 100.f)
		SetState(EEnemyMeleeState::Patrolling);
}

void ANecroLifeEnemyMelee::OnRep_MeleeState()
{
	// El AnimBP lee MeleeState directamente; nada extra necesario aquí
}

void ANecroLifeEnemyMelee::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANecroLifeEnemyMelee, MeleeState);
}
