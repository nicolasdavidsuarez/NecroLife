// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NecroLifeInterface.generated.h"

class ANecroLifeCharacter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNecroLifeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NECROLIFE_API INecroLifeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FVector GetLookAtLocation() const;

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	//void RequestCharacterLookAt(AActor* Actor, ANecroLifeCharacter* NecroLifeCharacter);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void OnInteract(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void RequestCharacterLookAt(ANecroLifeCharacter* CharacterToRotate);
};
