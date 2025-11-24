// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/QuestData.h"
#include "QuestComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NECROLIFE_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//Agregar quest
	void AddQuest(UQuestData* QuestData);

	//actualizar quests activas
	void ActualizarQuests();
	
	
	//FString Lista de tareas (ya editada)
	
    //void ingresar evento

private:
	TArray<UQuestData*> Quest;
	UQuestData* CurrentQuest;
	bool bAsQuest;
		
};
