// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/Public/UHealthComponent.h"
#include "Components/Public/AttributeComponent.h"
#include "NecroLifeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UHealthComponent;
class UAttributeComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class ANecroLifeCharacter : public ACharacter
{
	GENERATED_BODY()



	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* CameraBoomAction;

	/** dash Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* DashAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** Ability Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AbilityAction;
	/** Ability Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AbilityCancelAction;
    // Action
    UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Action;
	
	//distancia del puntero
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	float AbilityPointerMaxDistance = 300.f;
///////el puntero para usarlo en niagara 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability")
	FVector CachedAbilityPointer; // posición calculada cada frame
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

public:

	/** Constructor */
	ANecroLifeCharacter();	

protected:
	void SetBoomLength(const FInputActionValue& Value);
	void AbilityEnabled(const FInputActionValue& InputActionValue);
	void AbilityDisambled(const FInputActionValue& InputActionValue);
	void AplyAction();
	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:
	//componente de salud
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	UUHealthComponent* HealthComponent;
    //componente atributos
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	UAttributeComponent* Attribute;

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:
 FVector Direction;
 FRotator CurrentRotation,TargetRotation;

	//variables necesarias para dash
	bool bIsDashing = false;
	bool bCanDash = true;
	FTimerHandle DashTimerHandle;
	FTimerHandle CooldownTimerHandle;
	//
	
	FVector GetAbilityPointerPosition() const { return CachedAbilityPointer; }
	bool bEnabledAbility = false;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	
	void ShowMsg(FString Msg);
	///////////////////////
	///Puntero que esta frente a la direccion del character
	void UpdateAbilityPointer();
	////////////////////////////////////////////
	///Mirar al puntero cuando se castea habilidad
    void LookToCastAbility();

	//dash
	void Dash();
	void StopDash();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMapping;

	
};

