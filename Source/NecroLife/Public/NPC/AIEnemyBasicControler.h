// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnemyBasicControler.generated.h"




UCLASS()
class NECROLIFE_API AAIEnemyBasicControler : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAIEnemyBasicControler();

	UPROPERTY(EditAnywhere, Category="AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category="AI")
	UBlackboardData* BlackboardData;

	UPROPERTY(EditAnywhere, Category="AI")
	UBlackboardComponent* BlackboardComponent;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
