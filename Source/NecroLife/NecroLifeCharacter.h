// Copyright Epic Games, Inc. All Rights Reserved.


#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Public/Components/UHealthComponent.h"
#include "Public/Components/AttributeComponent.h"
#include "Public/Components/InventoryComponent.h"
#include "Public/Components/AbilityComponent.h"
#include "Engine/EngineTypes.h"
#include "Interface/NecroLifeInterface.h"
#include "Types/NecroLifeTypes.h"   
#include "NecroLifeCharacter.generated.h"


class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UUHealthComponent;
class UAttributeComponent;
class UInventoryComponent;
class UAbilityComponent;
class UQuestComponent;
class ANecroLifePlayerState;
//class INecroLifeInterface;
struct FInputActionValue;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMostrarInventario);
/**
*  A simple player-controllable third person character
*  Implements a controllable orbiting camera
*/
UCLASS(abstract)
class ANecroLifeCharacter : public ACharacter, public INecroLifeInterface
{
   GENERATED_BODY()






   /** Camera boom positioning the camera behind the character */
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
   USpringArmComponent* CameraBoom;


   /** Follow camera */
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
   UCameraComponent* FollowCamera;

public:
   UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Components")
   TObjectPtr<UBoxComponent> BoxCollision;

   // En QuestComponent.h (dentro de la clase UQuestComponent)
   UPROPERTY(BlueprintAssignable, Category = "Mostrar Inventario|UI")
   FMostrarInventario ShowInventory;
  
protected:

   //habilitar correr con el bloqueo de mayusculas
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* RunAction;

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

   //Interactuar
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* InteractAction;



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
   //curar si tiene pocion de cura
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* ApplyPosion;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* OpenInventory;
   
   //distancia del puntero
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
   float AbilityPointerMaxDistance = 300.f;
///////el puntero para usarlo en niagara
   UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ability")
   FVector CachedAbilityPointer; // posición calculada cada frame
  
   /** Look Input Action */
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* LookAction;

   //MOUSE DERECHO
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* MouseRightDown;
   //MOuse rigth up
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* MouseRightUp;

   //MOUSE medio
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* MouseMiddleDown;
   //MOuse rigth up
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* MouseMiddleUp;

   //Valores para la camara
   /*UPROPERTY(EditAnywhere, Category="Camera in game")
   float MaxDistanceToCursor=1200;
   UPROPERTY(EditAnywhere, Category="Camera in game")
   float MinDistanceToCursor=200;*/
   UPROPERTY(EditAnywhere, Category="Camera in game")
   float MaxPitch=-45;
   UPROPERTY(EditAnywhere, Category="Camera in game")
   float MinPitch=-25;
   UPROPERTY(EditAnywhere, Category="Camera in game")
   float MaxArmLenght=1200;
   UPROPERTY(EditAnywhere, Category="Camera in game")
   float MinArmLenght=200;


//Net
public:
   // Esta es la función que va a llamar el objeto cuando interactúes con él.
   // "Server" indica que viaja por red. "Reliable" asegura que el mensaje llegue sí o sí.
   UFUNCTION(Server, Reliable)
   void Server_ActualizarProgresoMision(FGameplayTag ObjectiveID, int32 Amount);
   UFUNCTION(Server, Reliable)
   void Server_AgregarMision(UQuestData* QuestData);

   /** Constructor */
   ANecroLifeCharacter();


protected:
   void SetBoomLength(const FInputActionValue& Value);
   void AbilityEnabled(const FInputActionValue& InputActionValue);
   void AbilityDisambled(const FInputActionValue& InputActionValue);
   void AplyAction();
   void OnRightMouseDown();
   void OnRightMouseUp();
   void OnMiddleMouseUp();
   void OnMiddleMouseDown();
   void RunActivated(const FInputActionValue& Value);
   void TakePosion();
   void Interact();
   void InventoryInput();
   /** Initialize input action bindings */
   virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
   UAttributeComponent* CachedAttributeComponent;

   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
   UInventoryComponent* CachedInventoryComponent;

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
    //componente inventario
   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
   UInventoryComponent* Inventory;
    //COmponente Habilidades
   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
   UAbilityComponent* Ability;


   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
   UQuestComponent* QuestComponent;

   

   
   UPROPERTY(BlueprintReadOnly, Category = "Components")
   TObjectPtr<UQuestComponent> CachedQuestComponent;
   
   UPROPERTY(BlueprintReadOnly, Category = "Player State")
   TObjectPtr<ANecroLifePlayerState> MyPlayerState;

protected:
   // Referencia genérica al Hub
   UPROPERTY(BlueprintReadWrite, Category = "UI")
   UUserWidget* HubWidget;
   
public:
   UFUNCTION(BlueprintCallable)
   FVector GetAbilityPointer() const {return CachedAbilityPointer;}
   
   /** Handles move inputs from either controls or UI interfaces */
   UFUNCTION(BlueprintCallable, Category="Input")
   virtual void DoMove(float Right, float Forward);


   /** Handles look inputs from either controls or UI interfaces */
   UFUNCTION(BlueprintCallable, Category="Input")
   virtual void DoLook(float Yaw, float Pitch);

   UFUNCTION(BlueprintCallable, Category="Input")
   virtual void LookAt(FVector TargetLocation);


   /** Handles jump pressed inputs from either controls or UI interfaces */
   UFUNCTION(BlueprintCallable, Category="Input")
   virtual void DoJumpStart();


   /** Handles jump pressed inputs from either controls or UI interfaces */
   UFUNCTION(BlueprintCallable, Category="Input")
   virtual void DoJumpEnd();

   UPROPERTY()
   AActor* CurrentInteractable; 
public:
   virtual void PossessedBy (AController* NewController) override;
   // El objeto llama a estas funciones desde su Overlap
   UFUNCTION(BLueprintCallable, Category="Interact")
   void SetCurrentInteractable(AActor* NewInteractable) { CurrentInteractable = NewInteractable; }
   UFUNCTION(BLueprintCallable, Category="Interact")
   void ClearCurrentInteractable() { CurrentInteractable = nullptr; }

   UFUNCTION(BLueprintCallable, Category="Interact")
  AActor *getCurrentInteractable() { return CurrentInteractable; }

   UFUNCTION(BLueprintCallable, Category="Interact")
   void AddCurrentQuest();

   UFUNCTION(BLueprintCallable, Category="Interact")
   void CancelCurrentQuest();
   
   UFUNCTION(BLueprintCallable, Category="Interact")
   bool ShowDialogue(FDialogLine CurrentLine);

   
   
FVector Direction;
FRotator CurrentRotation,TargetRotation;


   //variables necesarias para dash
   bool bIsDashing = false;
   bool bShowInventory=false;
   bool bCanDash = true;
   bool bMouseRightDown = false;
   bool bMouseMiddleDown = false;
   bool bIsRunning = false;
   FTimerHandle DashTimerHandle;
   FTimerHandle CooldownTimerHandle;
   //
  
   FVector GetAbilityPointerPosition() const { return CachedAbilityPointer; }
   bool bEnabledAbility = false;
   bool bLookAt = false;
  
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

   UFUNCTION(BlueprintCallable)
   void SetUIState(bool bIsTalking);

   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
   class UInputMappingContext* InputMapping;


   
  
};
