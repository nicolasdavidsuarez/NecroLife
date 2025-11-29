// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/QuestComponent.h"

#include <string>

#include "Data/QuestData.h"


// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	//ActualizarQuests();
	// ...
	
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		// ...
}

void UQuestComponent::AddQuest(UQuestData* QuestData)
{
if (Quests.Contains(QuestData))
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
	FString(TEXT("Ya contenia el data Asset")));			
	
}else{
	Quests.Add(QuestData);
	ActualizarQuests();
}
		//CurrentQuest = QuestData;		
}

void UQuestComponent::ActualizarQuests()
{
	
if (Quests.Num() == 0) return;

	for (int i = 0; i < Quests.Num(); i++)
	{
		if (ActiveQuest.Contains(Quests[i]))
		{
			if(ActiveQuest[i].bIsDone)
			{
				//lo tendria que sacar de la lista de misiones activas	
			}else{
				//esta cargada y activa	
			}
		}else{  //no esta en la lista de misiones cargadas y activas
			//agrego el quest data en la struct de FactiveQuestData
			FActiveQuestData NewActiveQuest;
			NewActiveQuest.QuestDataAsset = Quests[i];
			//de aca tendria que sacar la cantidad requerida Quests[i]->Objectives[j].AmountRequired
			NewActiveQuest.CurrentStage=0;
			for (int j=0;j<Quests[i]->Objectives.Num();j++)
			{
				NewActiveQuest.ObjectiveProgress.Add(Quests[i]->Objectives[j].TargetID,
					0);
			}
			ActiveQuest.Add(NewActiveQuest);		
		}
	}
	
	for (int i = 0; i < ActiveQuest.Num(); i++)
	{		
	for (int j=0;j<ActiveQuest[i].QuestDataAsset->Objectives.Num();j++)
	{
		if (ActiveQuest[i].CurrentStage==ActiveQuest[i].QuestDataAsset->Objectives[j].Stage)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			ActiveQuest[i].QuestDataAsset->Objectives[j].Description.ToString());
		}
	}
	}
	
}

// En UpdateQuestProgress
void UQuestComponent::UpdateQuestProgress(FGameplayTag ObjectiveID, int32 Amount)
{
	if (!ObjectiveID.IsValid()) return;

	// Usamos referencia (&) para poder modificar el struct dentro del array
	for (FActiveQuestData& ActiveQuestData : ActiveQuest)
	{
		// 1. Buscamos si esta quest tiene ese objetivo
		// Asumo que ObjectiveProgress guarda: <TagObjetivo, CantidadActual>
		int32* CurrentProgress = ActiveQuestData.ObjectiveProgress.Find(ObjectiveID);

		if (CurrentProgress)
		{
			// 2. CORRECCIÓN PUNTERO: Desreferenciar con *
			*CurrentProgress += Amount; 
            
			// Log para debug
			//UE_LOG(LogTemp, Warning, TEXT("Progreso actualizado: %d"), *CurrentProgress);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
						FString::FromInt(*CurrentProgress)); 
			// 3. Chequear si se completó (Pasamos la ActiveQuestData actual)
			CheckObjetivoComplete(ActiveQuestData, ObjectiveID);
		}
	}
}
/*
void UQuestComponent::UpdateQuestProgress(FGameplayTag ObjectiveID, int32 Amount)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			ObjectiveID.ToString()); 
	if (!ObjectiveID.IsValid()) return;
	for (FActiveQuestData& QuestData : ActiveQuest)
	{
		int32* CurrentValue = QuestData.ObjectiveProgress.Find(ObjectiveID);

		if (CurrentValue)
		{
					*CurrentValue += Amount;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			FString::FromInt(*CurrentValue));  //ObjectiveID.ToString()
		CheckObjetivoComplete(QuestData,ObjectiveID,Amount);
	}
	
}
*/

void UQuestComponent::CheckObjetivoComplete(FActiveQuestData& QuestToCheck, FGameplayTag ObjectiveID)
{
	// 1. Buscar cuánto necesitamos LEYENDO DEL DATA ASSET (que es estático)
	int32 RequiredAmount = 0;
	bool bFoundObjective = false;

	for (const auto& Objective : QuestToCheck.QuestDataAsset->Objectives)
	{
		if (Objective.TargetID == ObjectiveID)
		{
			RequiredAmount = Objective.AmountRequired;
			bFoundObjective = true;
			break;
		}
	}

	if (!bFoundObjective) return;

	// 2. Obtener cuanto tenemos actualmente
	int32 CurrentAmount = *QuestToCheck.ObjectiveProgress.Find(ObjectiveID);

	// 3. Comparar
	if (CurrentAmount >= RequiredAmount)
	{
		// 4. CORRECCIÓN DATA ASSET: Guardar estado en la estructura local, no en el asset
		// Asegúrate de agregar un bool bIsCompleted en tu struct FActiveQuestData
		// QuestToCheck.bIsCompleted = true; 
        
		QuestToCheck.CurrentStage++;
		UE_LOG(LogTemp, Warning, TEXT("¡Objetivo Completado! Nueva etapa: %d"), QuestToCheck.CurrentStage);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		FString("aca sumo el current stage"));
		// Aquí podrías llamar a una función que verifique si TODOS los objetivos de la quest están listos
	}
}

/*
void UQuestComponent::CheckObjetivoComplete(FActiveQuestData& QuestData, FGameplayTag ObjectiveID, int32 Amount)
{
	int cantidadNecesaria=0;
for (UQuestData* CurrentQuestEnLista:Quest){
	for (int i=0;i<CurrentQuestEnLista->Objectives.Num();i++)
	{
		if (CurrentQuestEnLista->Objectives[i].TargetID == ObjectiveID)
		{
		cantidadNecesaria=CurrentQuestEnLista->Objectives[i].AmountRequired;
		}
	}
}
	if (cantidadNecesaria<=Amount)
	{
		for (FActiveQuestData& QuestDataEnLista : ActiveQuest)
		{
			if (QuestDataEnLista.ObjectiveProgress.Contains(ObjectiveID))
			{
				//QuestDataEnLista.QuestDataAsset->bQuestDone=true;
				QuestDataEnLista.CurrentStage++;

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			FString("aca sumo el current stage"));
				ActualizarQuests();
			}
	}
	
	}
}*/


