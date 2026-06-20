// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/NPC/NecroLifeEnemyBossWolf.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/Components/RPGHelper.h"
#include "Engine/World.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

ANecroLifeEnemyBossWolf::ANecroLifeEnemyBossWolf()
{
	PrimaryActorTick.bCanEverTick = true;

	// Bug 2 fix: sin esto, PawnSensingComponent nunca dispara OnSeePawn
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bOnlySensePlayers = true;

	DashHitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DashHitSphere"));
	DashHitSphere->SetupAttachment(RootComponent);
	DashHitSphere->SetSphereRadius(100.f);
	DashHitSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DashHitSphere->SetGenerateOverlapEvents(false);

	EsenciasAlMorir = 500.f;
}

void ANecroLifeEnemyBossWolf::BeginPlay()
{
	Super::BeginPlay();

	BossStartLocation = GetActorLocation();
	PawnSensing->SightRadius = SightRadius;
	PawnSensing->SetPeripheralVisionAngle(VisionAngle);
	DashHitSphere->SetSphereRadius(DashHitRadius);

	if (HasAuthority())
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ANecroLifeEnemyBossWolf::OnSeePawn);
		DashHitSphere->OnComponentBeginOverlap.AddDynamic(this, &ANecroLifeEnemyBossWolf::OnDashHitSphereOverlap);
	}
}

void ANecroLifeEnemyBossWolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority() || BossState != EBossWolfState::Dashing) return;

	float Dist = FVector::Dist2D(GetActorLocation(), CurrentDashDestination);
	if (Dist <= DashArrivalThreshold)
	{
		OnDashArrived();
	}
	else
	{
		AddMovementInput(GetActorForwardVector(), 1.f);
	}
}

// --- Activación ---

void ANecroLifeEnemyBossWolf::OnSeePawn(APawn* Pawn)
{
	if (bIsDead || !HasAuthority() || bActivated || !Pawn || !Pawn->IsPlayerControlled()) return;

	bActivated = true;
	SetBossState(EBossWolfState::Howling);

	GetWorldTimerManager().SetTimer(HowlTimerHandle, this, &ANecroLifeEnemyBossWolf::SpawnWolves, HowlDuration, false);
}

// --- Fase: Spawn de lobos ---

void ANecroLifeEnemyBossWolf::SpawnWolves()
{
	if (bIsDead || !HasAuthority() || !WolfClass) return;

	TArray<FVector> Offsets = WolfSpawnOffsets;
	if (Offsets.Num() == 0)
	{
		Offsets = {
			FVector(200.f,   0.f, 0.f),
			FVector(-200.f,  0.f, 0.f),
			FVector(0.f,  200.f, 0.f),
			FVector(0.f, -200.f, 0.f)
		};
	}

	// Bug 4 fix: buscamos el jugador más cercano antes de spawnear
	// para setearlo como PlayerTarget en el Blackboard de cada lobo al instante
	APawn* NearestPlayer = nullptr;
	float MinDist = MAX_FLT;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			float Dist = FVector::Dist(GetActorLocation(), PC->GetPawn()->GetActorLocation());
			if (Dist < MinDist)
			{
				MinDist = Dist;
				NearestPlayer = PC->GetPawn();
			}
		}
	}

	AliveWolvesCount = 0;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < WolvesToSpawn; i++)
	{
		FVector Offset = Offsets[i % Offsets.Num()];
		FVector SpawnLoc = GetActorLocation() + Offset;
		SpawnLoc.Z = GetActorLocation().Z;

		// Bug 3 fix: spawn mirando al jugador más cercano en lugar de usar la rotación del boss
		FRotator SpawnRot = GetActorRotation();
		if (NearestPlayer)
		{
			FVector ToPlayer = (NearestPlayer->GetActorLocation() - SpawnLoc).GetSafeNormal2D();
			if (!ToPlayer.IsNearlyZero())
				SpawnRot = ToPlayer.Rotation();
		}

		ANecroLifeEnemyBasic* Wolf = GetWorld()->SpawnActor<ANecroLifeEnemyBasic>(
			WolfClass, SpawnLoc, SpawnRot, SpawnParams);

		if (Wolf)
		{
			AliveWolvesCount++;
			Wolf->OnEnemyDied.AddDynamic(this, &ANecroLifeEnemyBossWolf::OnWolfDied);

			// Bug 4 fix: setear PlayerTarget en el Blackboard inmediatamente
			if (NearestPlayer)
			{
				AAIController* WolfAI = Cast<AAIController>(Wolf->GetController());
				if (WolfAI && WolfAI->GetBlackboardComponent())
				{
					WolfAI->GetBlackboardComponent()->SetValueAsObject(FName("PlayerTarget"), NearestPlayer);
				}
			}
		}
	}

	SetBossState(EBossWolfState::WaitingForWolves);

	if (AliveWolvesCount == 0)
	{
		StartFreeDamagePhase();
	}
}

void ANecroLifeEnemyBossWolf::OnWolfDied(ANecroLifeEnemyBasic* Wolf)
{
	if (!HasAuthority() || BossState != EBossWolfState::WaitingForWolves) return;

	AliveWolvesCount = FMath::Max(0, AliveWolvesCount - 1);
	if (AliveWolvesCount == 0)
	{
		StartFreeDamagePhase();
	}
}

// --- Fase: Daño libre ---

void ANecroLifeEnemyBossWolf::StartFreeDamagePhase()
{
	SetBossState(EBossWolfState::FreeDamage);
	GetCharacterMovement()->StopMovementImmediately();

	GetWorldTimerManager().SetTimer(FreeDamageTimerHandle, this, &ANecroLifeEnemyBossWolf::StartCharging, FreeDamageTime, false);
}

// --- Fase: Carga ---

void ANecroLifeEnemyBossWolf::StartCharging()
{
	if (bIsDead) return;

	SetBossState(EBossWolfState::Charging);

	GetWorldTimerManager().SetTimer(ChargeTimerHandle, this, &ANecroLifeEnemyBossWolf::StartDashing, ChargeDuration, false);
}

// --- Fase: Dash ---

void ANecroLifeEnemyBossWolf::StartDashing()
{
	if (bIsDead) return;

	if (!DashTargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANecroLifeEnemyBossWolf: DashTargetActor no asignado en el nivel."));
		return;
	}

	DamagedActorsDuringDash.Empty();

	CurrentDashDestination = bNextDashToTarget
		? DashTargetActor->GetActorLocation()
		: BossStartLocation;

	bNextDashToTarget = !bNextDashToTarget;

	FVector Dir = (CurrentDashDestination - GetActorLocation()).GetSafeNormal2D();
	if (!Dir.IsNearlyZero())
	{
		SetActorRotation(Dir.Rotation());
	}

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
	DashHitSphere->SetGenerateOverlapEvents(true);

	SetBossState(EBossWolfState::Dashing);
}

void ANecroLifeEnemyBossWolf::OnDashArrived()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->bOrientRotationToMovement = true;
	DashHitSphere->SetGenerateOverlapEvents(false);
	DamagedActorsDuringDash.Empty();

	StartPostDash();
}

// --- Fase: Post-dash (gira, espera, spawna lobos) ---

void ANecroLifeEnemyBossWolf::StartPostDash()
{
	SetBossState(EBossWolfState::PostDash);

	FRotator TurnedRot = GetActorRotation();
	TurnedRot.Yaw += 180.f;
	SetActorRotation(TurnedRot);

	GetWorldTimerManager().SetTimer(PostDashTimerHandle, this, &ANecroLifeEnemyBossWolf::SpawnWolves, PostDashWaitTime, false);
}

// --- Daño durante el dash ---

void ANecroLifeEnemyBossWolf::OnDashHitSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || !OtherActor || DamagedActorsDuringDash.Contains(OtherActor)) return;

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn || !Pawn->IsPlayerControlled()) return;

	DamagedActorsDuringDash.Add(OtherActor);
	URPGHelper::ApplyDamage(OtherActor, DashDamage);
}

// --- Replicación y estado ---

void ANecroLifeEnemyBossWolf::SetBossState(EBossWolfState NewState)
{
	if (!HasAuthority() || bIsDead) return;
	BossState = NewState;
	OnRep_BossState();
}

void ANecroLifeEnemyBossWolf::OnRep_BossState()
{
	switch (BossState)
	{
	case EBossWolfState::Howling:          BP_OnHowl();       break;
	case EBossWolfState::FreeDamage:       BP_OnFreeDamage(); break;
	case EBossWolfState::Charging:         BP_OnCharge();     break;
	case EBossWolfState::Dashing:          BP_OnDash();       break;
	case EBossWolfState::Idle:
	case EBossWolfState::WaitingForWolves:
	case EBossWolfState::PostDash:         BP_OnIdle();       break;
	}
}

void ANecroLifeEnemyBossWolf::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANecroLifeEnemyBossWolf, BossState);
}
