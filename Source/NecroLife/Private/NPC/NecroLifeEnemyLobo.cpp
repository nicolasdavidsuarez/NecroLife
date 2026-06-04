// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/NPC/NecroLifeEnemyLobo.h"
#include "Net/UnrealNetwork.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ANecroLifeEnemyLobo::ANecroLifeEnemyLobo()
{
	EsenciasAlMorir = 75.f;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->bOnlySensePlayers = true;
}

void ANecroLifeEnemyLobo::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->SightRadius = SightRadius;
	PawnSensing->SetPeripheralVisionAngle(VisionAngle);
	GetCharacterMovement()->MaxWalkSpeed = IdleSpeed;

	if (HasAuthority())
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ANecroLifeEnemyLobo::OnSeePawn);
	}
}

void ANecroLifeEnemyLobo::OnSeePawn(APawn* Pawn)
{
	if (bIsDead || !HasAuthority() || !Pawn || !Pawn->IsPlayerControlled()) return;

	AAIController* AIC = Cast<AAIController>(GetController());
	if (!AIC) return;

	UBlackboardComponent* BB = AIC->GetBlackboardComponent();
	if (!BB) return;

	BB->SetValueAsObject(FName("PlayerTarget"), Pawn);

	if (!bIsChasing)
	{
		bIsChasing = true;
		GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
		OnRep_IsChasing();
	}
}

void ANecroLifeEnemyLobo::OnRep_IsChasing()
{
	BP_OnStartChase();
}

void ANecroLifeEnemyLobo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANecroLifeEnemyLobo, bIsChasing);
}
