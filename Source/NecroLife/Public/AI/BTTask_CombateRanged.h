// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CombateRanged.generated.h"

struct FBTCombateRangedMemory
{
	float ShootCooldown;
	float ShootPause;
	float TimeOutOfRange;
	bool  bFleeing;
};

UCLASS()
class NECROLIFE_API UBTTask_CombateRanged : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CombateRanged();

	UPROPERTY(EditAnywhere, Category="Combate Ranged")
	float ShootCooldownDuration = 2.0f;

	UPROPERTY(EditAnywhere, Category="Combate Ranged")
	float ShootPauseDuration = 1.0f;

	UPROPERTY(EditAnywhere, Category="Combate Ranged")
	float LoseTargetTime = 3.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTCombateRangedMemory); }
};
