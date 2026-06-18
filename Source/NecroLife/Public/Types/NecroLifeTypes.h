#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NecroLifeTypes.generated.h" 

USTRUCT(BlueprintType)
struct FDialogLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText SpeakerName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (MultiLine = true))
	FText DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	class UAnimMontage* SpeakerAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	bool bIsMissionChoice;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	bool bRequiresPreviousObjective;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	bool bIsInfoOnly;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredStage = 0;
	
	// Constructor para inicializar valores
	FDialogLine()
	{
		SpeakerName = FText::FromString("NPC");
		DialogueText = FText::GetEmpty();
		SpeakerAnim = nullptr;
		bIsMissionChoice = false;
		bRequiresPreviousObjective = false;
		bIsInfoOnly = false;
		RequiredStage=0;		
	}
};