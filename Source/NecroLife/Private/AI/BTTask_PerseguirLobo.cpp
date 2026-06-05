// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/AI/BTTask_PerseguirLobo.h"
#include "Public/NPC/NecroLifeEnemyLobo.h"
#include "Public/Components/RPGHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_PerseguirLobo::UBTTask_PerseguirLobo()
{
	NodeName = TEXT("Perseguir Lobo");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PerseguirLobo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BB) return EBTNodeResult::Failed;

	ANecroLifeEnemyLobo* Lobo = Cast<ANecroLifeEnemyLobo>(AIC->GetPawn());
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(FName("PlayerTarget")));
	if (!Lobo || !IsValid(Target)) return EBTNodeResult::Failed;

	FBTPerseguirLoboMemory* Memory = reinterpret_cast<FBTPerseguirLoboMemory*>(NodeMemory);
	Memory->AttackCooldown = 0.f;
	Memory->MoveUpdateTimer = 0.f;

	return EBTNodeResult::InProgress;
}

void UBTTask_PerseguirLobo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BB)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ANecroLifeEnemyLobo* Lobo = Cast<ANecroLifeEnemyLobo>(AIC->GetPawn());
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(FName("PlayerTarget")));

	if (!Lobo || !IsValid(Target))
	{
		BB->ClearValue(FName("PlayerTarget"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FBTPerseguirLoboMemory* Memory = reinterpret_cast<FBTPerseguirLoboMemory*>(NodeMemory);
	Memory->AttackCooldown  -= DeltaSeconds;
	Memory->MoveUpdateTimer -= DeltaSeconds;

	float Distance = FVector::Dist(Lobo->GetActorLocation(), Target->GetActorLocation());

	if (Distance <= Lobo->AttackRange)
	{
		AIC->StopMovement();

		FVector ToTarget = Target->GetActorLocation() - Lobo->GetActorLocation();
		Lobo->SetActorRotation(FRotator(0.f, ToTarget.Rotation().Yaw, 0.f));

		if (Memory->AttackCooldown <= 0.f)
		{
			Lobo->BP_OnAttack();
			URPGHelper::ApplyDamage(Target, Lobo->AttackDamage);
			Memory->AttackCooldown = Lobo->AttackCooldown;
		}
	}
	else
	{
		if (Memory->MoveUpdateTimer <= 0.f)
		{
			AIC->MoveToActor(Target, Lobo->AttackRange * 0.5f);
			Memory->MoveUpdateTimer = MoveUpdateInterval;
		}
	}
}

EBTNodeResult::Type UBTTask_PerseguirLobo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC) AIC->StopMovement();
	return EBTNodeResult::Aborted;
}
