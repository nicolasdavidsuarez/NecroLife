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
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"


ANecroLifeCharacter::ANecroLifeCharacter()
{
   // crear y atachar el UHealthComponent
   HealthComponent = CreateDefaultSubobject<UUHealthComponent>(TEXT("HealthComponent"));
   //crear y atachar Inventario
   Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
//crear el UAbilityComponent
   Ability = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
   
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
   CameraBoom->bUsePawnControlRotation = true;
   CameraBoom->bInheritPitch = true;
   CameraBoom->bInheritYaw=true;


   // Create a follow camera
   FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
   FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
   FollowCamera->bUsePawnControlRotation = false;


   Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributesComponent"));
  
   // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
   // are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
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
   //ShowMsg(AbilityName);
   if (!bEnabledAbility)
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
Ability->SelectAbility(pressedKeys);
         if (Ability && Ability->CurrentAbility)
         {
            Ability->UpdateIndicator(HitResult.Location);
         }
      }
  
      bEnabledAbility = true;      
   }
   Ability->InitPreview();
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
    
     // DrawDebugCone(GetWorld(),GetActorLocation(),,450.0f,0.5,0.5,12,FColor::Blue,false,0.1f);
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

      if (!bHit) return;

      for (auto& Result : Overlaps)
      {
         AActor* Other = Result.GetActor();
         if (!Other || Other == this) continue;

         // 🔹 Vector hacia el otro actor
         FVector ToTarget = (Other->GetActorLocation() - Origin).GetSafeNormal();

         // 🔹 Calculamos el ángulo con el forward vector
         float Dot = FVector::DotProduct(Forward, ToTarget);
         float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(Dot));

         // 🔹 Si está dentro del cono, aplicamos daño
         if (AngleToTarget <= AttackAngle)
         {
            //UGameplayStatics::ApplyDamage(Other, 20.f, GetController(), this, UDamageType::StaticClass());
            
            URPGHelper::ApplyDamage(Other,100);

            // 🔹 (Opcional) debug line
            DrawDebugLine(GetWorld(), Origin, Other->GetActorLocation(), FColor::Red, false, 1.f, 0, 1.f);
         }
      }

      // 🔹 Debug del área del ataque
      
      //ShowMsg(FString::Printf(TEXT("Ability Ejecuted")));
      GetCharacterMovement()->bOrientRotationToMovement = true;
      bUseControllerRotationYaw = false;
      bEnabledAbility = false;
      Ability->ClearIndicator();
   }else
   {
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
   if (Inventory->UseHealtPosion())
   {
      HealthComponent->ApplyHealing(30.0f);
   }else
   {
   //   ShowMsg(FString::Printf(TEXT("don´t have poison")));
   }
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
      EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::AbilityEnabled);
      EnhancedInputComponent->BindAction(AbilityCancelAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::AbilityDisambled);
      ///Dash//////////////////
      EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Dash);
      //correr////////////
      EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::RunActivated);
      ///CUrar si tiene pociones//////////////////
      EnhancedInputComponent->BindAction(ApplyPosion, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::TakePosion);
      //ACTION!!!
      EnhancedInputComponent->BindAction(Action, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::AplyAction);
   }
   else
   {
      UE_LOG(LogNecroLife, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
   }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ANecroLifeCharacter::Move(const FInputActionValue& Value)
{
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
   }
   if (GetController() != nullptr&&bMouseMiddleDown)
   {
      //ShowMsg(FString::Printf(TEXT("middle button activa el pitch")));
      // AddControllerPitchInput(Pitch);
      //CameraBoom->SetRelativeRotation(FRotator(FMath::GetMappedRangeValueClamped(FVector2D(100,1200),FVector2D(-10,-45),armLength), 0.0f, 0.0f));
      //  CameraBoom->GetRelativeRotation().Pitch;
      
      if(CameraBoom->GetRelativeRotation().Pitch+Pitch>=MaxPitch&&CameraBoom->GetRelativeRotation().Pitch+Pitch<=MinPitch)
      {
         CameraBoom->AddRelativeRotation(FRotator(Pitch,0.0f,0.0f));
         //ShowMsg(FString::Printf(TEXT("Pitch: %f"),CameraBoom->GetRelativeRotation().Pitch));
      
      }else{
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
