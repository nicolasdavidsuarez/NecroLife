// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/NPC/AIEnemyBasicControler.h"

#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
AAIEnemyBasicControler::AAIEnemyBasicControler()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIEnemyBasicControler::BeginPlay()
{
	Super::BeginPlay();
	
	if (BehaviorTree && BlackboardData)
	{
		UseBlackboard(BlackboardData, BlackboardComponent);
		RunBehaviorTree(BehaviorTree);
	}
}

// Called every frame
void AAIEnemyBasicControler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

