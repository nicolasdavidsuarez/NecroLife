// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "LANConnexion.generated.h"

/**
 * 
 */
UCLASS()
class NECROLIFE_API ULANConnexion : public UUserWidget
{
	
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Create;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Join;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_Ip;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> World;

	UFUNCTION()
	void OnButtonCreateClicked();
	
	UFUNCTION()
	void OnButtonJoinClicked();

	
};
