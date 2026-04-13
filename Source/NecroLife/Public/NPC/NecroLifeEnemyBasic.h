// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Public/Components/UHealthComponent.h"
#include "NecroLifeEnemyBasic.generated.h"


class UUHealthComponent;


UCLASS()
class NECROLIFE_API ANecroLifeEnemyBasic : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANecroLifeEnemyBasic();

	// En NecroLifeEnemyBasic.h
	UPROPERTY(ReplicatedUsing = OnRep_IsDead)
	bool bIsDead = false;

	UFUNCTION()
	void OnRep_IsDead();
	void Die();

	//componente de salud
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	UUHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	FGameplayTag Tag;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetTag()
	{
		return Tag;
	}

	UFUNCTION(BlueprintCallable)
	void SetTag(FGameplayTag newTag)
	{
		Tag=newTag;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool IsAlive();

protected:
	// Esta función es obligatoria para replicar variables
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
