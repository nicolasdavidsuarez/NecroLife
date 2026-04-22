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
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMostrarInventario);

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

   // Inputs
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* RunAction;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* JumpAction;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* MoveAction;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* CameraBoomAction;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* DashAction;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* InteractAction;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* MouseLookAction;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* AbilityAction;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* AbilityCancelAction;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* Action;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* ApplyPosion;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* OpenInventory;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* LookAction;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* MouseRightDown;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* MouseRightUp;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* MouseMiddleDown;
   UPROPERTY(EditAnywhere, Category="Input")
   UInputAction* MouseMiddleUp;

   // Distancia del puntero
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
   float AbilityPointerMaxDistance = 300.f;
   
   // El puntero para usarlo en niagara
   UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ability")
   FVector CachedAbilityPointer; 
  
   // Valores para la camara
   UPROPERTY(EditAnywhere, Category="Camera in game")
   float MaxPitch=-45;
   UPROPERTY(EditAnywhere, Category="Camera in game")
   float MinPitch=-25;
   UPROPERTY(EditAnywhere, Category="Camera in game")
   float MaxArmLenght=1200;
   UPROPERTY(EditAnywhere, Category="Camera in game")
   float MinArmLenght=200;

   // --- SISTEMA DE COMBATE Y ANIMACIONES (Tests_VFX) ---

   // Referencia al arma para poder modificarla desde C++
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Equipment")
   class UMeshComponent* WeaponMesh;

   // Efecto visual (Niagara) que spawnea al impactar a un enemigo
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX")
   class UNiagaraSystem* HitVFX;

   // Guarda el ID del Root Motion para el Dash
   uint16 DashRootMotionID;

   // --- MONTAGES DE ANIMACIÓN DASH ---
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combate|Animaciones")
   UAnimMontage* DashMontage;

   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combate|Animaciones")
   UAnimMontage* DashAttackMontage;

   // --- SISTEMA DE COMBOS ---
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
   bool bIsAttacking;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
   int32 AttackCount;

   // Lista ordenada de animaciones (Golpe 1, Golpe 2, Golpe 3, etc.)
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animations")
   TArray<UAnimMontage*> ComboMontages;

   // Controla el tiempo que tiene el jugador antes de que el combo vuelva a cero
   FTimerHandle ComboResetTimer;

   // Reinicia todo el estado a 0
   void ResetCombo();

   // --- MATERIALES ORIGINALES PARA INVISIBILIDAD DEL DASH ---
   UPROPERTY()
   TArray<class UMaterialInterface*> OriginalMaterials;

   UPROPERTY()
   TArray<class UMaterialInterface*> OriginalWeaponMaterials;

public:
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
   virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
   UAttributeComponent* CachedAttributeComponent;

   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
   UInventoryComponent* CachedInventoryComponent;

   void Move(const FInputActionValue& Value);
   void Look(const FInputActionValue& Value);

   // --- RED DASH: Funciones de red para el dash ---
   UFUNCTION(Server, Reliable)
   void Server_Dash(FVector DashDir);

   // --- Red dash - animaciones de comabte ---
   UFUNCTION(Server, Reliable)
   void Server_PlayCombatMontage(UAnimMontage* MontageToPlay);

   UFUNCTION(NetMulticast, Unreliable)
   void Multicast_PlayCombatMontage(UAnimMontage* MontageToPlay);

   UFUNCTION(NetMulticast, Unreliable)
   void Multicast_DashFX();

   UFUNCTION(NetMulticast, Reliable)
   void Multicast_StopDashFX();

   // Lógica física central del Dash
   void PerformDashLogic(FVector DashDir);
   
   // Lógica puramente visual
   void Local_DashFX();

public:
   // Componentes
   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
   UUHealthComponent* HealthComponent;
   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
   UAttributeComponent* Attribute;
   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
   UInventoryComponent* Inventory;
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
   
   UFUNCTION(BlueprintCallable, Category="Input")
   virtual void DoMove(float Right, float Forward);
   UFUNCTION(BlueprintCallable, Category="Input")
   virtual void DoLook(float Yaw, float Pitch);
   UFUNCTION(BlueprintCallable, Category="Input")
   virtual void LookAt(FVector TargetLocation);
   UFUNCTION(BlueprintCallable, Category="Input")
   virtual void DoJumpStart();
   UFUNCTION(BlueprintCallable, Category="Input")
   virtual void DoJumpEnd();

   UPROPERTY()
   AActor* CurrentInteractable; 

public:
   virtual void PossessedBy (AController* NewController) override;
   
   UFUNCTION(BlueprintCallable, Category="Interact")
   void SetCurrentInteractable(AActor* NewInteractable) { CurrentInteractable = NewInteractable; }
   UFUNCTION(BlueprintCallable, Category="Interact")
   void ClearCurrentInteractable() { CurrentInteractable = nullptr; }
   UFUNCTION(BlueprintCallable, Category="Interact")
   AActor *getCurrentInteractable() { return CurrentInteractable; }
   UFUNCTION(BlueprintCallable, Category="Interact")
   void AddCurrentQuest();
   UFUNCTION(BlueprintCallable, Category="Interact")
   void CancelCurrentQuest();
   UFUNCTION(BlueprintCallable, Category="Interact")
   bool ShowDialogue(FDialogLine CurrentLine);

   // --- FUNCIONES DE ATAQUE (Tests_VFX) ---
   UFUNCTION(BlueprintCallable, Category = "Combat")
   void ExecuteAttackHit();

   UFUNCTION(BlueprintCallable, Category = "Combat")
   void ExecuteAbilityHit();

   UFUNCTION(BlueprintCallable, Category = "Combat|State")
   void ResetAttackState();

   // Efecto visual para el Dash
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX")
   class UNiagaraSystem* DashVFX;
       
   // Sonido para el Dash
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Audio")
   class USoundBase* DashSound;

   // Material transparente para el efecto de invisibilidad
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX")
   class UMaterialInterface* TransparentMaterial;

   FVector Direction;
   FRotator CurrentRotation,TargetRotation;

   // Estados
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
   bool bIsInvincible;
   UPROPERTY(BlueprintReadWrite, Category = "Combate|Estados") 
   bool bIsDashing = false;

   bool bShowInventory=false;
   bool bCanDash = true;
   bool bMouseRightDown = false;
   bool bMouseMiddleDown = false;
   bool bIsRunning = false;
   FTimerHandle DashTimerHandle;
   FTimerHandle CooldownTimerHandle;
  
   FVector GetAbilityPointerPosition() const { return CachedAbilityPointer; }
   bool bEnabledAbility = false;
   bool bLookAt = false;
  
   FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
   FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

   virtual void BeginPlay() override;
   virtual void Tick(float DeltaTime) override;
   
   // Sobreescribimos la función nativa de Unreal para la invencibilidad (Tests_VFX)
   virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

   void ShowMsg(FString Msg);
   void UpdateAbilityPointer();
   void LookToCastAbility();

   void Dash();
   void StopDash();

   UFUNCTION(BlueprintCallable)
   void SetUIState(bool bIsTalking);

   // Se llama automáticamente cuando RecalcularEstadisticas termina (M1-Base)
   UFUNCTION()
   void OnAtributosActualizados(const FEstadisticasPersonaje& NuevosAtributos);

   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
   class UInputMappingContext* InputMapping;
};