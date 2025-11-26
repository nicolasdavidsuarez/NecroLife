// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/QuestComponent.h"
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
	ActualizarQuests();
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
if (Quest.Contains(QuestData))
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
	FString(TEXT("Ya contenia el data Asset")));			
	
}else{
	Quest.Add(QuestData);
	ActualizarQuests();
}
		//CurrentQuest = QuestData;		
}

void UQuestComponent::ActualizarQuests()
{
	
if (Quest.Num() == 0) return;

	for (int i = 0; i < Quest.Num(); i++)
	{
		if (Quest[i]->bQuestDone)
		{
			
		}else{
			FActiveQuestData NewActiveQuest;
			NewActiveQuest.QuestDataAsset = Quest[i];
			NewActiveQuest.CurrentStage=0;
			for (int j=0;j<Quest[i]->Objectives.Num();j++)
			{
				NewActiveQuest.ObjectiveProgress.Emplace(Quest[i]->Objectives[j].TargetID,
					Quest[i]->Objectives[j].AmountRequired);
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

void UQuestComponent::UpdateQuestProgress(FGameplayTag ObjectiveID, int32 Amount)
{
	if (!ObjectiveID.IsValid()) return;
	for (FActiveQuestData& QuestData : ActiveQuest)
	{
		int32* CurrentValue = QuestData.ObjectiveProgress.Find(ObjectiveID);

		if (CurrentValue)
		{
					Amount += Amount;
		}
		CheckObjetivoComplete(QuestData,ObjectiveID,Amount);
	}
	
}

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
				QuestDataEnLista.QuestDataAsset->bQuestDone=true;
				QuestDataEnLista.CurrentStage++;

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			FString("aca sumo el current stage"));
				ActualizarQuests();
			}
	}
	
	}
}


