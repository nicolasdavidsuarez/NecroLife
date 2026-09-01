// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PerseguirLobo.generated.h"

struct FBTPerseguirLoboMemory
{
	float AttackCooldown;
	float MoveUpdateTimer;
};

UCLASS()
class NECROLIFE_API UBTTask_PerseguirLobo : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PerseguirLobo();

	UPROPERTY(EditAnywhere, Category="Lobo")
	float MoveUpdateInterval = 0.4f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTPerseguirLoboMemory); }
};
