// Copyright Epic Games, Inc. All Rights Reserved.


#include "NecroLifePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "NecroLife.h"
#include "NecroLifeCharacter.h"
#include "Components/InventoryComponent.h"
#include "Widgets/NecroLifeHud.h"
#include "Widgets/Input/SVirtualJoystick.h"

void ANecroLifePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogNecroLife, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void ANecroLifePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

void ANecroLifePlayerController::RefrescarInventarioVisual(const TArray<FDatosGema>& DatosGemas)
{
	if (!IsLocalController()) return;
	ANecroLifeCharacter* MiChar = Cast<ANecroLifeCharacter>(GetPawn());

	
	if (MiChar&&MiChar->HubWidget)
	{
		UNecroLifeHud* MiHUD = Cast<UNecroLifeHud>(MiChar->HubWidget);
		MiHUD->ActualizarInventario(DatosGemas);
	}	
}

void ANecroLifePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	UInventoryComponent* InvComp = InPawn->FindComponentByClass<UInventoryComponent>();
	if (InvComp)
	{		
		InvComp->GemsToShow.AddDynamic(this, &ANecroLifePlayerController::RefrescarInventarioVisual);
		InvComp->GemsToShow.AddDynamic(this, &ANecroLifePlayerController::Client_RefrescarInventarioVisual);
	}
}

void ANecroLifePlayerController::Client_RefrescarInventarioVisual_Implementation(const TArray<FDatosGema>& DatosGemas)
{
	ANecroLifeCharacter* MiChar = Cast<ANecroLifeCharacter>(GetPawn());

	
	if (MiChar&&MiChar->HubWidget)
	{
		UNecroLifeHud* MiHUD = Cast<UNecroLifeHud>(MiChar->HubWidget);
		MiHUD->ActualizarInventario(DatosGemas);
	}	
}