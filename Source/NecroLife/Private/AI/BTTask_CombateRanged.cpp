// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/AI/BTTask_CombateRanged.h"
#include "Public/NPC/NecroLifeEnemyRanged.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_CombateRanged::UBTTask_CombateRanged()
{
	NodeName = TEXT("Combate Ranged");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_CombateRanged::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BB) return EBTNodeResult::Failed;

	ANecroLifeEnemyRanged* Enemy = Cast<ANecroLifeEnemyRanged>(AIController->GetPawn());
	AActor* PlayerTarget = Cast<AActor>(BB->GetValueAsObject(FName("PlayerTarget")));
	if (!Enemy || !PlayerTarget) return EBTNodeResult::Failed;

	// Control manual de rotacion — el movimiento no debe girar al enemigo
	if (UCharacterMovementComponent* MovComp = Enemy->GetCharacterMovement())
		MovComp->bOrientRotationToMovement = false;

	FBTCombateRangedMemory* Memory = reinterpret_cast<FBTCombateRangedMemory*>(NodeMemory);
	Memory->ShootCooldown  = 0.f;
	Memory->MoveCooldown   = 0.f;
	Memory->ShootPause     = 0.f;
	Memory->TimeOutOfRange = 0.f;
	Memory->bFleeing       = false;

	return EBTNodeResult::InProgress;
}

void UBTTask_CombateRanged::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BB)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ANecroLifeEnemyRanged* Enemy = Cast<ANecroLifeEnemyRanged>(AIController->GetPawn());
	AActor* PlayerTarget = Cast<AActor>(BB->GetValueAsObject(FName("PlayerTarget")));
	if (!Enemy || !PlayerTarget)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// Mirar al jugador en todo momento (huida o no)
	FVector ToPlayer = PlayerTarget->GetActorLocation() - Enemy->GetActorLocation();
	Enemy->SetActorRotation(FRotator(0.f, ToPlayer.Rotation().Yaw, 0.f));

	float Distance = FVector::Dist(Enemy->GetActorLocation(), PlayerTarget->GetActorLocation());

	FBTCombateRangedMemory* Memory = reinterpret_cast<FBTCombateRangedMemory*>(NodeMemory);
	Memory->ShootCooldown -= DeltaSeconds;
	Memory->MoveCooldown  -= DeltaSeconds;

	// Pausa post-disparo: enemigo quieto mirando al jugador
	if (Memory->ShootPause > 0.f)
	{
		Memory->ShootPause -= DeltaSeconds;
		return;
	}

	// Huida: retroceder si el jugador está demasiado cerca
	if (Distance < Enemy->FleeDistance)
	{
		if (Memory->MoveCooldown <= 0.f)
		{
			FVector DirAway = (Enemy->GetActorLocation() - PlayerTarget->GetActorLocation()).GetSafeNormal();
			FVector RetreatPoint = Enemy->GetActorLocation() + DirAway * 400.f;
			AIController->MoveToLocation(RetreatPoint, 50.f);
			Memory->MoveCooldown = 0.3f;
			Memory->bFleeing = true;
		}
	}
	else
	{
		if (Memory->bFleeing)
		{
			AIController->StopMovement();
			Memory->bFleeing = false;
		}
	}

	// Si el jugador está fuera de rango, contar tiempo y volver a patrullar
	if (Distance > Enemy->ShootRange)
	{
		Memory->TimeOutOfRange += DeltaSeconds;
		if (Memory->TimeOutOfRange >= LoseTargetTime)
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	Memory->TimeOutOfRange = 0.f;

	// Disparo: para en seco, dispara, y queda 1s quieto antes de retomar la huida
	if (Distance <= Enemy->ShootRange && Memory->ShootCooldown <= 0.f)
	{
		AIController->StopMovement();
		Memory->bFleeing = false;
		Enemy->ShootProjectile(PlayerTarget);
		Memory->ShootCooldown = ShootCooldownDuration;
		Memory->ShootPause    = ShootPauseDuration;
	}
}

EBTNodeResult::Type UBTTask_CombateRanged::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		AIController->StopMovement();

		if (ANecroLifeEnemyRanged* Enemy = Cast<ANecroLifeEnemyRanged>(AIController->GetPawn()))
			if (UCharacterMovementComponent* MovComp = Enemy->GetCharacterMovement())
				MovComp->bOrientRotationToMovement = true;
	}
	return EBTNodeResult::Aborted;
}
