// Copyright Epic Games, Inc. All Rights Reserved.


#include "NecroLifeCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "NecroLife.h"
#include "Public/Components/AttributeComponent.h"
#include "Public/Components/RPGHelper.h"
#include "CollisionQueryParams.h"
#include "NecroLifeGameState.h"
#include "NecroLifePlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/QuestComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/PlayerState.h"
#include "Interface/NecroLifeInterface.h"
#include "NPC/NecroLifeEnemyBasic.h"
#include "NPC/NecroLifeNpcBasic.h"

//NET
void ANecroLifeCharacter::Server_ActualizarProgresoMision_Implementation(FGameplayTag ObjectiveID, int32 Amount)
{
   // 1. Buscamos el GameState de nuestra partida
   // (Como esta función corre en el Servidor, GetGameState siempre será válido y tendrá la última información)
   ANecroLifeGameState* GS = GetWorld()->GetGameState<ANecroLifeGameState>();

   if (GS && GS->QuestManager)
   {
      // 2. Le pasamos la pelota al Quest Manager que ahora vive ahí
      GS->QuestManager->UpdateQuestProgress(ObjectiveID, Amount);
        
      // Opcional: Un log para confirmar que el servidor lo recibió
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("El Servidor recibió la interacción y actualizó el GameState"));
   }
}

void ANecroLifeCharacter::Server_AgregarMision_Implementation(UQuestData* QuestData)
{
   // 1. Buscamos el GameState (que es la autoridad)
   ANecroLifeGameState* GS = GetWorld()->GetGameState<ANecroLifeGameState>();

   // 2. Si existe el GameState y tiene nuestro QuestManager
   if (GS && GS->QuestManager)
   {
      // 3. Agregamos la misión de forma global para todos
      GS->QuestManager->AddQuest(QuestData);
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ANecroLifeCharacter::ANecroLifeCharacter()
{
   // crear y atachar el UHealthComponent
   HealthComponent = CreateDefaultSubobject<UUHealthComponent>(TEXT("HealthComponent"));
   //crear y atachar Inventario
   Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
//crear el UAbilityComponent
   Ability = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
   //crear y atachar el quest component
   QuestComponent= CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));
   //
   Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributesComponent"));

   
   // Set size for collision capsule
   GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
     
   // Don't rotate when the controller rotates. Let that just affect the camera.
   bUseControllerRotationPitch = false;
   bUseControllerRotationYaw = false;
   bUseControllerRotationRoll = false;


   // Configure character movement
   GetCharacterMovement()->bOrientRotationToMovement = true;
   GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);


   // Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
   // instead of recompiling to adjust them
   GetCharacterMovement()->JumpZVelocity = 500.f;
   GetCharacterMovement()->AirControl = 0.35f;
   GetCharacterMovement()->MaxWalkSpeed = 500.f;
   GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
   GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
   GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


   // Create a camera boom (pulls in towards the player if there is a collision)
   CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
   CameraBoom->SetupAttachment(RootComponent);
   
   CameraBoom->TargetArmLength = 800.0f;
   CameraBoom->bDoCollisionTest=true;
   
   CameraBoom->CameraLagSpeed=2.0f;
   CameraBoom->bUsePawnControlRotation = true;
   CameraBoom->bInheritPitch = true;
   CameraBoom->bInheritYaw=true;


   // Create a follow camera
   FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
   FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
   FollowCamera->bUsePawnControlRotation = false;



   BoxCollision=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCol"));
   BoxCollision->SetBoxExtent(FVector(100,100,100),true);


   //Network
   bReplicates=true;
   SetReplicatingMovement(true);
   GetMesh()->SetIsReplicated(true);
}
///////////////////////////////////////////////////////////////////////////////////////////
void ANecroLifeCharacter::SetBoomLength(const FInputActionValue& Value)
{
   FVector2D InputVector = Value.Get<FVector2D>();
   float armLength = CameraBoom->TargetArmLength;
  
   if (InputVector.X>0&&armLength<MaxArmLenght)
   {
      armLength += CameraBoom->TargetArmLength*0.05f;
      CameraBoom->TargetArmLength = armLength;
      //FMath::GetMappedRangeValueClamped(FVector2D(100,1200),FVector2D(-45,-10),armLength)
      //CameraBoom->add
      //CameraBoom->SetRelativeRotation(FRotator(FMath::GetMappedRangeValueClamped(FVector2D(100,1200),FVector2D(-10,-45),armLength), 0.0f, 0.0f));
   }
   else
   {     
      armLength -= CameraBoom->TargetArmLength*0.05f;
      if (armLength>MinArmLenght)
      {
         CameraBoom->TargetArmLength = armLength;
         //  CameraBoom->SetRelativeRotation(FRotator(FMath::GetMappedRangeValueClamped(FVector2D(100,1200),FVector2D(-10,-45),armLength), 0.0f, 0.0f));
      }
   }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ANecroLifeCharacter::AbilityEnabled(const FInputActionValue& InputActionValue)
{

   int32 pressedKeys = static_cast<int32>(InputActionValue.Get<float>())-1;
   FString AbilityName =FString("se entra en modo combate " + FString::FromInt(pressedKeys));
   ShowMsg(AbilityName);
   Ability->SelectAbility(pressedKeys);
     
   
   
   if (!Ability->isCoolDownAply(Ability->CurrentAbility))
      {
         GetCharacterMovement()->bOrientRotationToMovement = false;
      bUseControllerRotationYaw = true;
      FHitResult HitResult;
  
      APlayerController* PC = Cast<APlayerController>(GetController());
      if (PC && PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
      {
     
         FVector TargetLocation = HitResult.Location;
         Direction = TargetLocation - GetActorLocation();
         Direction.Z = 0;
         CurrentRotation = GetActorRotation();
         TargetRotation = Direction.Rotation();
         //Controller->SetControlRotation(NewRotation);
         //  DrawDebugLine(GetWorld(),GetActorLocation(),TargetLocation,FColor::Emerald,
         //   false,3.0f,1,10);
         //Ability->SelectAbility(pressedKeys);
         if (Ability && Ability->CurrentAbility)
         {
            Ability->UpdateIndicator(HitResult.Location);
         }
      }
  
      bEnabledAbility = true;      
     Ability->InitPreview();

}else
{
   //aca iria si la habilidad esta en cool down, por ahora no hace nada. no se si hacer que se vea distinta
   //o que reproduzca sonido de eeee!
   
}
   
}

void ANecroLifeCharacter::AbilityDisambled(const FInputActionValue& InputActionValue)
{
   if (bEnabledAbility)
   {
   //   ShowMsg(FString::Printf(TEXT("Se Cancela Habilidad")));
      GetCharacterMovement()->bOrientRotationToMovement = true;
      bUseControllerRotationYaw = false;
      bEnabledAbility = false;
      Ability->ClearIndicator();
   }
  
      bLookAt=true;
  
}


void ANecroLifeCharacter::AplyAction()
{
   if (bEnabledAbility)
   {
       //FOverlapResult result;
      // 🔹 Parámetros del cono
      float AttackRadius = 300.f;        // Distancia del ataque
      float AttackAngle = 45.f;          // Mitad del ángulo del cono (en grados)
    
      FVector Origin = GetActorLocation();
      FVector Forward = GetActorForwardVector();

      // 🔹 Buscamos actores cercanos con una esfera
      TArray<FOverlapResult> Overlaps;
      FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackRadius);

      bool bHit = GetWorld()->OverlapMultiByChannel(
             Overlaps,
             Origin,
             FQuat::Identity,
             ECC_Pawn,          // Canal de colisión 
             CollisionShape
         );

      GetCharacterMovement()->bOrientRotationToMovement = true;
      bUseControllerRotationYaw = false;
      bEnabledAbility = false;
      Ability->AbilityAply();
      Ability->ClearIndicator();

      if (!bHit) return;

      for (auto& Result : Overlaps)
      {
         AActor* Other = Result.GetActor();
         ANecroLifeEnemyBasic* EnemyBasic=Cast<ANecroLifeEnemyBasic>(Other);
         if (!EnemyBasic || Other == this) continue;

         // 🔹 Vector hacia el otro actor
         FVector ToTarget = (EnemyBasic->GetActorLocation() - Origin).GetSafeNormal();

         // 🔹 Calculamos el ángulo con el forward vector
         float Dot = FVector::DotProduct(Forward, ToTarget);
         float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(Dot));

         // 🔹 Si está dentro del cono, aplicamos daño
         if (AngleToTarget <= AttackAngle)
         {
            //UGameplayStatics::ApplyDamage(Other, 20.f, GetController(), this, UDamageType::StaticClass());
            
            URPGHelper::ApplyDamage(Other,100);
            if (!EnemyBasic->IsAlive())
            {
             //  ShowMsg(FString::Printf(TEXT("Aca Sumaria experiencia")));
               URPGHelper::TakeXP(this,10);
               //QuestComponent->UpdateQuestProgress(EnemyBasic->GetTag(),1);
               Server_ActualizarProgresoMision(EnemyBasic->GetTag(),1);
            }
            // 🔹 (Opcional) debug line
            DrawDebugLine(GetWorld(), Origin, Other->GetActorLocation(), FColor::Red, false, 1.f, 0, 1.f);
            
         }
      }

      // 🔹 Debug del área del ataque
      
      //ShowMsg(FString::Printf(TEXT("Ability Ejecuted")));
      
     
   }else
   {
      TArray<FOverlapResult> Overlaps;
      FVector Origin = GetActorLocation();
      FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(100,100,100));

      bool bHit = GetWorld()->OverlapMultiByChannel(
                   Overlaps,
                   Origin,
                   FQuat::Identity,
                   ECC_Pawn,          // Canal de colisión 
                   CollisionShape
               );
      if (!bHit) return;
      FVector Forward = GetActorForwardVector();
      DrawDebugCone(GetWorld(),GetActorLocation(),Forward,100.0f,0.5,0.5,12,FColor::Red,false,0.5f);

      
      for (auto& Result : Overlaps)
      {
         AActor* Other = Result.GetActor();
         ANecroLifeEnemyBasic* EnemyBasic=Cast<ANecroLifeEnemyBasic>(Other);
         if (!EnemyBasic || Other == this) continue;

         // 🔹 Vector hacia el otro actor
         FVector ToTarget = (EnemyBasic->GetActorLocation() - Origin).GetSafeNormal();

         // 🔹 Calculamos el ángulo con el forward vector
         float Dot = FVector::DotProduct(Forward, ToTarget);
         float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(Dot));
         float AttackAngle = 45.f;  

         // 🔹 Si está dentro del cono, aplicamos daño
         if (AngleToTarget <= AttackAngle)
         {
            //UGameplayStatics::ApplyDamage(Other, 20.f, GetController(), this, UDamageType::StaticClass());
            
            URPGHelper::ApplyDamage(Other,10);
            if (!EnemyBasic->IsAlive())
            {
               ShowMsg(FString::Printf(TEXT("Aca Sumaria experiencia")));
               URPGHelper::TakeXP(this,10);
            }
            // 🔹 (Opcional) debug line
            DrawDebugLine(GetWorld(), Origin, Other->GetActorLocation(), FColor::Red, false, 1.f, 0, 1.f);
            //Ability->AbilityAply();
         }
      }
      //ShowMsg(FString::Printf(TEXT("Se ejecuta Ataque Melee")));
   }
     
}

void ANecroLifeCharacter::OnRightMouseDown()
{
   bMouseRightDown = true;
}

void ANecroLifeCharacter::OnRightMouseUp()
{
   bMouseRightDown = false;
}

void ANecroLifeCharacter::OnMiddleMouseUp()
{
   bMouseMiddleDown = false;
}

void ANecroLifeCharacter::OnMiddleMouseDown()
{
   bMouseMiddleDown = true;
}

void ANecroLifeCharacter::RunActivated(const FInputActionValue& Value)
{
   if (Value.Get<bool>())
   {
      //ShowMsg(TEXT("se cambia de estado de correr a trotar"));
      bIsRunning =!bIsRunning;
      GetCharacterMovement()->MaxWalkSpeed = bIsRunning? 2000.0f:500.0f;
   }
   /*  if (bIsRunning)
     {
        GetCharacterMovement()->MaxWalkSpeed = 500.f;
        bIsRunning = false;
     }else{
        GetCharacterMovement()->MaxWalkSpeed = 1500.f;
        bIsRunning = true;
     }*/      
}

void ANecroLifeCharacter::TakePosion()
{
   if (MyPlayerState)
   {
   if (CachedInventoryComponent->UseHealtPosion())
   {
      HealthComponent->ApplyHealing(30.0f);
   }else
   {
         ShowMsg(FString::Printf(TEXT("don´t have poison")));
   }
}
}
///////////Para cuando apreta boton de interactuar, "t" de talk
void ANecroLifeCharacter::Interact()
{
   // Si tenemos un objeto guardado y ese objeto usa nuestra interfaz
   if (CurrentInteractable&&CurrentInteractable->Implements<UNecroLifeInterface>())
   {
      // Ejecutamos la función (esto hará que el objeto lance su lógica y el PJ lo mire)
      UE_LOG(LogTemp, Warning, TEXT("implementa on interact"));
   INecroLifeInterface::Execute_OnInteract(CurrentInteractable,this);
      

   }else
   {
      UE_LOG(LogTemp, Warning, TEXT("no hay nadie con quien interactuar, y se presiono la t "));
   }
    
}



void ANecroLifeCharacter::InventoryInput()
{

   if (!IsLocallyControlled())
   {
      return;
   }
   if (!bShowInventory)
   {
      bShowInventory=true;
      SetUIState(true);
      ShowInventory.Broadcast();
   }else
   {
      SetUIState(false);
      bShowInventory=false;
      ShowInventory.Broadcast();
   }
}

void ANecroLifeCharacter::AddCurrentQuest()
{
//   ANecroLifeNpcBasic* NpcBasic=Cast<ANecroLifeNpcBasic>(CurrentInteractable);
   //QuestComponent->AddQuest(NpcBasic->QuestActual);   
  // NpcBasic->NextAddQuest();
   // Hacemos el Cast y verificamos que no sea nulo (buena práctica en C++)
   if (ANecroLifeNpcBasic* NpcBasic = Cast<ANecroLifeNpcBasic>(CurrentInteractable))
   {
      // 1. En lugar de llamar al componente local, le avisamos al Servidor
      Server_AgregarMision(NpcBasic->QuestActual);
        
      // 2. Esto lo dejamos tal cual. Asumo que NextAddQuest() cierra la interfaz 
      // gráfica o avanza el diálogo del NPC en la pantalla del jugador.
      NpcBasic->NextAddQuest(); 
   }
}

void ANecroLifeCharacter::CancelCurrentQuest()
{
   if (ANecroLifeNpcBasic* NpcBasic = Cast<ANecroLifeNpcBasic>(CurrentInteractable))
   {
      NpcBasic->CancelAddQuest();
   }
}

bool ANecroLifeCharacter::ShowDialogue(FDialogLine CurrentLine)
{
   
   if (HubWidget)
   {
      UFunction* Func = HubWidget->FindFunction(FName("ShowDialogueLine"));
      if (Func)
      {
         HubWidget->ProcessEvent(Func, &CurrentLine);
         // Esto te dirá en el log si el nombre está mal escrito o no se encuentra
         UE_LOG(LogTemp, Warning, TEXT("se encontró la función ShowDialogueLine en el Hub"));
         return CurrentLine.bIsMissionChoice;
      }
      else 
      {
         // Esto te dirá en el log si el nombre está mal escrito o no se encuentra
         UE_LOG(LogTemp, Warning, TEXT("No se encontró la función ShowDialogueLine en el Hub"));
      }
   }
   else 
   {
      UE_LOG(LogTemp, Error, TEXT("HubWidget es NULO en el Character"));
   }
   
   return false;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
void ANecroLifeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
   // Set up action bindings
   if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
     
      // Jumping
      EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
      EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
      // Moving
      EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Move);
      EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Look);
      // Looking
      // EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Look);

      EnhancedInputComponent->BindAction(MouseRightDown,ETriggerEvent::Triggered,this,&ANecroLifeCharacter::OnRightMouseDown);
      EnhancedInputComponent->BindAction(MouseRightUp,ETriggerEvent::Triggered,this,&ANecroLifeCharacter::OnRightMouseUp);

      EnhancedInputComponent->BindAction(MouseMiddleDown,ETriggerEvent::Triggered,this,&ANecroLifeCharacter::OnMiddleMouseDown);
      EnhancedInputComponent->BindAction(MouseMiddleUp,ETriggerEvent::Triggered,this,&ANecroLifeCharacter::OnMiddleMouseUp);

      EnhancedInputComponent->BindAction(CameraBoomAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::SetBoomLength);
      EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &ANecroLifeCharacter::AbilityEnabled);
      EnhancedInputComponent->BindAction(AbilityCancelAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::AbilityDisambled);
      EnhancedInputComponent->BindAction(OpenInventory, ETriggerEvent::Started, this, &ANecroLifeCharacter::InventoryInput);
      ///Dash//////////////////
      EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Dash);
//interactuar
      EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Interact);
      //correr////////////
      EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::RunActivated);
      ///CUrar si tiene pociones//////////////////
      EnhancedInputComponent->BindAction(ApplyPosion, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::TakePosion);
      //ACTION!!!
      EnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, this, &ANecroLifeCharacter::AplyAction);
   }
   else
   {
      UE_LOG(LogNecroLife, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
   }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ANecroLifeCharacter::Move(const FInputActionValue& Value)
{
   if (bShowInventory)
   {
      return;  //esto es porque se movia el player 1 cuando habria el inventario
   }
   // input is a Vector2D
   FVector2D MovementVector = Value.Get<FVector2D>();
   // route the input
   DoMove(MovementVector.X, MovementVector.Y);
}


void ANecroLifeCharacter::Look(const FInputActionValue& Value)
{
   // input is a Vector2D
   FVector2D LookAxisVector = Value.Get<FVector2D>();
   // route the input
   DoLook(LookAxisVector.X, LookAxisVector.Y);
}


void ANecroLifeCharacter::DoLook(float Yaw, float Pitch)
{
   if (GetController() != nullptr&&bMouseRightDown)
   {
      // add yaw and pitch input to controller
      AddControllerYawInput(Yaw);
     //AddControllerPitchInput(Pitch);
      //bLookAt=false;
   
 //  if (GetController() != nullptr&&bMouseMiddleDown)
   //{
      //ShowMsg(FString::Printf(TEXT("middle button activa el pitch")));
      // AddControllerPitchInput(Pitch);
      //CameraBoom->SetRelativeRotation(FRotator(FMath::GetMappedRangeValueClamped(FVector2D(100,1200),FVector2D(-10,-45),armLength), 0.0f, 0.0f));
      //  CameraBoom->GetRelativeRotation().Pitch;
      
      if(CameraBoom->GetRelativeRotation().Pitch+Pitch>=MaxPitch&&CameraBoom->GetRelativeRotation().Pitch+Pitch<=MinPitch)
      {
         CameraBoom->AddRelativeRotation(FRotator(Pitch,0.0f,0.0f));
         //ShowMsg(FString::Printf(TEXT("Pitch: %f"),CameraBoom->GetRelativeRotation().Pitch));
      
      }else
         {
         if (Pitch!=0)
         {
            Pitch*=-1.0f;
            CameraBoom->AddRelativeRotation(FRotator(Pitch,0.0f,0.0f));
         }
         //ShowMsg(FString::Printf(TEXT("Pitch: %f"),Pitch));
      }
   }
  // AddControllerPitchInput(Pitch);
}

void ANecroLifeCharacter::LookAt(FVector TargetLocation)
{
    FVector StartLocation = GetPawnViewLocation(); 

   FVector LookDirection = TargetLocation - StartLocation;
   FRotator LookAtRot = LookDirection.Rotation();

   DoLook(LookAtRot.Yaw, LookAtRot.Pitch);
   //para que no crashee despues lo analizo
   if (AController* CharController = GetController())
   {
      CharController->SetControlRotation(LookAtRot);
   }
   //GetController()->SetControlRotation(LookAtRot);
}


void ANecroLifeCharacter::DoMove(float Right, float Forward)
{
   if (GetController() != nullptr)
   {
      // find out which way is forward
      const FRotator Rotation = GetController()->GetControlRotation();
      const FRotator YawRotation(0, Rotation.Yaw, 0);


      // get forward vector
      const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);


      // get right vector
      const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


      // add movement
      AddMovementInput(ForwardDirection, Forward);
      AddMovementInput(RightDirection, Right);
     
   // AddMovementInput(GetActorForwardVector(), Forward);
   // AddMovementInput(GetActorForwardVector(), Right);
   }
}




void ANecroLifeCharacter::DoJumpStart()
{
   // signal the character to jump
   Jump();
}


void ANecroLifeCharacter::DoJumpEnd()
{
   // signal the character to stop jumping
   StopJumping();
}

void ANecroLifeCharacter::PossessedBy(AController* NewController)
{
   Super::PossessedBy(NewController);
   // 1. Cacheamos el PlayerState como antes
   MyPlayerState = GetPlayerState<ANecroLifePlayerState>();
   if (MyPlayerState)
   {
      // 2. AHORA, usamos MyPlayerState para cachear sus componentes
      CachedAttributeComponent = MyPlayerState->GetAttributeComponent();
      CachedInventoryComponent = MyPlayerState->GetInventoryComponent();
      CachedQuestComponent = MyPlayerState->GetQuestComponent();

      // (Opcional pero recomendado) Comprobar que todos los componentes se encontraron
      if (!CachedAttributeComponent || !CachedInventoryComponent || !CachedQuestComponent)
      {
         UE_LOG(LogTemp, Error, TEXT("Character %s failed to cache all components from PlayerState."), *GetName());
      }
   }
   else
   {
      UE_LOG(LogTemp, Error, TEXT("Character %s possessed but failed to get PlayerState."), *GetName());
   }
}


void ANecroLifeCharacter::BeginPlay()
{
   Super::BeginPlay();


   APlayerController* PC = Cast<APlayerController>(GetController());
   if (PC)
   {
      if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
      {
         if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
         {
            if (InputMapping) // asegurate de asignarlo en el editor
            {
               Subsystem->AddMappingContext(InputMapping, 0);
            }
         }
      }
   }
}


void ANecroLifeCharacter::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
  
   UpdateAbilityPointer();
   LookToCastAbility();
  
}


void ANecroLifeCharacter::ShowMsg(FString Msg)
{
   if (GEngine)
   {
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
            Msg);
   }
}


void ANecroLifeCharacter::UpdateAbilityPointer()
{
   APlayerController* PC = Cast<APlayerController>(GetController());
   if (!PC) return;


   FHitResult HitResult;
   if (PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
   {
      FVector PlayerPos = GetActorLocation();
      FVector ToMouse = HitResult.Location - PlayerPos;
      ToMouse.Z = 0;


      if (ToMouse.Size() > AbilityPointerMaxDistance)
      {
         ToMouse = ToMouse.GetSafeNormal() * AbilityPointerMaxDistance;
      }


      CachedAbilityPointer = PlayerPos + ToMouse;


      // Debug para verlo
      // Aca habria que dibujar lo que quede en el juego mas adelante
      //DrawDebugLine(GetWorld(), PlayerPos, CachedAbilityPointer, FColor::Green, false, -1.f, 0, 2.f);
      DrawDebugSphere(GetWorld(), CachedAbilityPointer, 20.f, 12, FColor::Red, false, -1.f);
      CachedAbilityPointer.Z = PlayerPos.Z;
      if (bEnabledAbility)
       {
          Ability->UpdatePreview(CachedAbilityPointer);
       }
   }
}


void ANecroLifeCharacter::LookToCastAbility()
{
   // Interpola suavemente hacia la rotación deseada
   if (bEnabledAbility)
   {
      //FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 100.0f);
      // el último número es la velocidad de rotación (5.0f es moderado)
     
      //SetActorRotation(NewRotation);
      FVector PlayerPos = GetActorLocation();
      FVector ToMouse = CachedAbilityPointer - PlayerPos;
      ToMouse.Z = 0;
      SetActorRotation(ToMouse.Rotation());
      //DrawDebugCone(GetWorld(),PlayerPos,ToMouse,450.0f,0.5,0.5,12,FColor::Blue,false,0.1f);
    
   }
}


void ANecroLifeCharacter::Dash()
{
   if (!bCanDash || bIsDashing || !Attribute) return;
   bIsDashing = true;
   bCanDash = false;
   FVector DashDirection = GetActorForwardVector();
   LaunchCharacter(DashDirection * Attribute->DashStrength, true, true);


   GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ANecroLifeCharacter::StopDash, Attribute->DashDuration, false,0.5f);
}


void ANecroLifeCharacter::StopDash()
{
   bIsDashing = false;
   GetWorldTimerManager().SetTimer(CooldownTimerHandle, [this]()
   {
      bCanDash = true;
   }, Attribute->DashCooldown, false);
}
void ANecroLifeCharacter::SetUIState(bool bIsTalking)
{
   APlayerController* PC = Cast<APlayerController>(GetController());
   if (PC)
   {
      if (bIsTalking)
      {
         // Bloqueamos el input de movimiento
         PC->SetInputMode(FInputModeGameAndUI());
         PC->bShowMouseCursor = true;
        // GetCharacterMovement()->DisableMovement();  desde el bIsTalking desabilitamos el moviemiento
        // en la funcion move(). cuando es true sale antes de mover. dejo la camara(el look()) prendida por que me parece bien
      }
      else
      {
         // Devolvemos el control al juego
         PC->SetInputMode(FInputModeGameOnly());
         PC->bShowMouseCursor = false;
        // GetCharacterMovement()->SetMovementMode(MOVE_Walking);
      }
   }
}