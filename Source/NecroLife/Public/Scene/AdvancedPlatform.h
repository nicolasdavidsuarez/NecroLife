// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "AdvancedPlatform.generated.h"


UENUM(BlueprintType)
enum class EPlatformState:uint8
{
	Hidden,
	FadingIn,
	Visible,
	FadingOut	
};

UCLASS()
class NECROLIFE_API AAdvancedPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAdvancedPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> PlatformMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> PlatformDoors;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialInstanceDynamic> Material ;

	UPROPERTY(EditAnywhere)
	FVector PlatformClose;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditAnywhere,Category="Movement")
	float Speed=200.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector StartPoint;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector EndPoint;

	UPROPERTY(EditAnywhere,Category="Movement")
	TArray<FVector> Path;

	UPROPERTY(EditAnywhere, Category = "Fade")
    float FadeDuration=2.f; 

	void FadeIn();
	void FadeOut();
	void UpdateOpacity();
	
private:
	EPlatformState State;
	float CurrentOpacity;
	
	bool bIsOverlappingPlatform=false;
	int Nodo=0;
};
