#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Types/NecroLifeTypes.h" // Importante: Incluimos tu struct
#include "NecroLifeDialogData.generated.h"

UCLASS(BlueprintType)
class NECROLIFE_API UNecroLifeDialogData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TArray<FDialogLine> DialogLines;
};