// Fill out your copyright notice in the Description page of Project Settings.
// item base que contiene el overlap y el endvoerlap

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BaseItem.generated.h"

class USphereComponent;
class StaticMeshComponent;


UCLASS()
class NECROLIFE_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	//////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SphereMesh;

	UFUNCTION(BlueprintCallable)
	void ShowMsg(FString Msg);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
