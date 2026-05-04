// Copyright Epic Games, Inc. All Rights Reserved.

#include "NecroLifeCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "NecroLife.h"
#include "Public/Components/AttributeComponent.h"
#include "Public/Components/RPGHelper.h"
#include "CollisionQueryParams.h"
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


/////////////////// CHARACTER ///////////////////
ANecroLifeCharacter::ANecroLifeCharacter()
{
   HealthComponent = CreateDefaultSubobject<UUHealthComponent>(TEXT("HealthComponent"));
   Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
   Ability = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
   QuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));
   Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributesComponent"));
   
   GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
     
   bUseControllerRotationPitch = false;
   bUseControllerRotationYaw = false;
   bUseControllerRotationRoll = false;

   GetCharacterMovement()->bOrientRotationToMovement = true;
   GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
   GetCharacterMovement()->JumpZVelocity = 500.f;
   GetCharacterMovement()->AirControl = 0.35f;
   GetCharacterMovement()->MaxWalkSpeed = 50.f;
   GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
   GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
   GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

   CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
   CameraBoom->SetupAttachment(RootComponent);
   CameraBoom->TargetArmLength = 800.0f;
   CameraBoom->bDoCollisionTest = true;
   CameraBoom->CameraLagSpeed = 2.0f;
   CameraBoom->bUsePawnControlRotation = true;
   CameraBoom->bInheritPitch = true;
   CameraBoom->bInheritYaw = true;

   FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
   FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
   FollowCamera->bUsePawnControlRotation = false;

   BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCol"));
   BoxCollision->SetBoxExtent(FVector(100,100,100), true);

   bReplicates = true;
   SetReplicatingMovement(true);
   GetMesh()->SetIsReplicated(true);
}

/////////////////// CÁMARA & MOVIMIENTO ///////////////////
void ANecroLifeCharacter::SetBoomLength(const FInputActionValue& Value)
{
   FVector2D InputVector = Value.Get<FVector2D>();
   float armLength = CameraBoom->TargetArmLength;
  
   if (InputVector.X > 0 && armLength < MaxArmLenght)
   {
      armLength += CameraBoom->TargetArmLength * 0.05f;
      CameraBoom->TargetArmLength = armLength;
   }
   else
   {     
      armLength -= CameraBoom->TargetArmLength * 0.05f;
      if (armLength > MinArmLenght)
      {
         CameraBoom->TargetArmLength = armLength;
      }
   }
}

void ANecroLifeCharacter::Move(const FInputActionValue& Value)
{
   if (bShowInventory) return;
   FVector2D MovementVector = Value.Get<FVector2D>();
   DoMove(MovementVector.X, MovementVector.Y);
}

void ANecroLifeCharacter::Look(const FInputActionValue& Value)
{
   FVector2D LookAxisVector = Value.Get<FVector2D>();
   DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ANecroLifeCharacter::DoLook(float Yaw, float Pitch)
{
   if (GetController() != nullptr && bMouseRightDown)
   {
      AddControllerYawInput(Yaw);
      if(CameraBoom->GetRelativeRotation().Pitch + Pitch >= MaxPitch && CameraBoom->GetRelativeRotation().Pitch + Pitch <= MinPitch)
      {
         CameraBoom->AddRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));
      }
      else
      {
         if (Pitch != 0)
         {
            Pitch *= -1.0f;
            CameraBoom->AddRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));
         }
      }
   }
}

void ANecroLifeCharacter::LookAt(FVector TargetLocation)
{
   FVector StartLocation = GetPawnViewLocation(); 
   FVector LookDirection = TargetLocation - StartLocation;
   FRotator LookAtRot = LookDirection.Rotation();
   DoLook(LookAtRot.Yaw, LookAtRot.Pitch);
   GetController()->SetControlRotation(LookAtRot);
}

void ANecroLifeCharacter::DoMove(float Right, float Forward)
{
   if (GetController() != nullptr)
   {
      const FRotator Rotation = GetController()->GetControlRotation();
      const FRotator YawRotation(0, Rotation.Yaw, 0);
      const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
      const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

      AddMovementInput(ForwardDirection, Forward);
      AddMovementInput(RightDirection, Right);
   }
}

void ANecroLifeCharacter::DoJumpStart() { Jump(); }
void ANecroLifeCharacter::DoJumpEnd() { StopJumping(); }

void ANecroLifeCharacter::OnRightMouseDown() { bMouseRightDown = true; }
void ANecroLifeCharacter::OnRightMouseUp() { bMouseRightDown = false; }
void ANecroLifeCharacter::OnMiddleMouseDown() { bMouseMiddleDown = true; }
void ANecroLifeCharacter::OnMiddleMouseUp() { bMouseMiddleDown = false; }

void ANecroLifeCharacter::RunActivated(const FInputActionValue& Value)
{
   if (Value.Get<bool>())
   {
      bIsRunning = !bIsRunning;
      GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? Attribute->Velocity * 200.0f : Attribute->Velocity * 50.0f;
   }     
}

/////////////////// COMBATE Y HABILIDADES ///////////////////
void ANecroLifeCharacter::AbilityEnabled(const FInputActionValue& InputActionValue)
{
   int32 pressedKeys = static_cast<int32>(InputActionValue.Get<float>()) - 1;
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
         if (Ability && Ability->CurrentAbility)
         {
            Ability->UpdateIndicator(HitResult.Location);
         }
      }
      bEnabledAbility = true;      
      Ability->InitPreview();
   }
}

void ANecroLifeCharacter::AbilityDisambled(const FInputActionValue& InputActionValue)
{
   if (bEnabledAbility)
   {
      GetCharacterMovement()->bOrientRotationToMovement = true;
      bUseControllerRotationYaw = false;
      bEnabledAbility = false;
      Ability->ClearIndicator();
   }
   bLookAt = true;
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
        CachedAbilityPointer.Z = PlayerPos.Z;

        if (bEnabledAbility)
        {
            Ability->UpdatePreview(CachedAbilityPointer);
        }
    }
}

void ANecroLifeCharacter::LookToCastAbility()
{
    if (bEnabledAbility)
    {
        FVector PlayerPos = GetActorLocation();
        FVector ToMouse = CachedAbilityPointer - PlayerPos;
        ToMouse.Z = 0;
        SetActorRotation(ToMouse.Rotation());
    }
}

void ANecroLifeCharacter::AplyAction()   
{
    if (bShowInventory || CurrentInteractable) return;

    if (bEnabledAbility)
    {
        GetCharacterMovement()->bOrientRotationToMovement = true;
        bUseControllerRotationYaw = false;
        bEnabledAbility = false;
        Ability->ClearIndicator();

        if (Ability->CurrentAbility && Ability->CurrentAbility->AbilityMontage)
        {
            PlayAnimMontage(Ability->CurrentAbility->AbilityMontage);
        	Server_PlayCombatMontage(Ability->CurrentAbility->AbilityMontage);
        }
    }
    else if (bIsDashing)
    {
        if (DashAttackMontage)
        {
            StopDash();
            bIsAttacking = true;
            PlayAnimMontage(DashAttackMontage);
        	Server_PlayCombatMontage(DashAttackMontage);
            AttackCount = 0;
        }
    }
    else
    {
        if (bIsAttacking || ComboMontages.Num() == 0) return;

        bIsAttacking = true;
        UAnimMontage* MontageToPlay = ComboMontages[AttackCount];
        if (MontageToPlay)
        {
            PlayAnimMontage(MontageToPlay);
        	Server_PlayCombatMontage(MontageToPlay); // Le aviso a los demás
        }

        AttackCount++;
        if (AttackCount >= ComboMontages.Num())
        {
            AttackCount = 0;
        }

        GetWorldTimerManager().SetTimer(ComboResetTimer, this, &ANecroLifeCharacter::ResetCombo, 2.5f, false);
    }
}

void ANecroLifeCharacter::ResetCombo()
{
    AttackCount = 0;
    bIsAttacking = false;
}

void ANecroLifeCharacter::ResetAttackState()
{
    bIsAttacking = false;
}

void ANecroLifeCharacter::ExecuteAbilityHit()
{
    float AttackRadius = 300.f;
    float AttackAngle = 45.f;

    FVector Origin = GetActorLocation();
    FVector Forward = GetActorForwardVector();
    TArray<FOverlapResult> Overlaps;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackRadius);

    bool bHit = GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn, CollisionShape);
    Ability->AbilityAply();

    if (!bHit) return;

    for (auto& Result : Overlaps)
    {
        AActor* Other = Result.GetActor();
        ANecroLifeEnemyBasic* EnemyBasic = Cast<ANecroLifeEnemyBasic>(Other);
        if (!EnemyBasic || Other == this) continue;

        FVector ToTarget = (EnemyBasic->GetActorLocation() - Origin).GetSafeNormal();
        float Dot = FVector::DotProduct(Forward, ToTarget);
        float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(Dot));

        if (AngleToTarget <= AttackAngle)
        {
            URPGHelper::ApplyDamage(Other, Attribute->Attack);
            if (!EnemyBasic->IsAlive())
            {
                URPGHelper::TakeXP(this, 10);
            	QuestComponent->UpdateQuestProgress(EnemyBasic->GetTag(), 1);
            }
        }
    }
}

void ANecroLifeCharacter::ExecuteAttackHit()
{
    TArray<FOverlapResult> Overlaps;
    FVector Origin = GetActorLocation();
    FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(100, 100, 100));

    bool bHit = GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn, CollisionShape);

    if (!bHit) return;
    FVector Forward = GetActorForwardVector();

    for (auto& Result : Overlaps)
    {
        AActor* Other = Result.GetActor();
        ANecroLifeEnemyBasic* EnemyBasic = Cast<ANecroLifeEnemyBasic>(Other);
        if (!EnemyBasic || Other == this) continue;

        FVector ToTarget = (EnemyBasic->GetActorLocation() - Origin).GetSafeNormal();
        float Dot = FVector::DotProduct(Forward, ToTarget);
        float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(Dot));
        float AttackAngle = 45.f;

        if (AngleToTarget <= AttackAngle)
        {
            URPGHelper::ApplyDamage(Other, Attribute->Attack);
            if (HitVFX)
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, Other->GetActorLocation());
            }

            if (!EnemyBasic->IsAlive())
            {
                URPGHelper::TakeXP(this, 10);
            	QuestComponent->UpdateQuestProgress(EnemyBasic->GetTag(), 1);
            }
        }
    }
}

float ANecroLifeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsInvincible)
    {
        return 0.0f;
    }
    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

/////////////////// DASH ///////////////////
void ANecroLifeCharacter::Dash()
{
	if (!bCanDash || bIsDashing || bIsAttacking || !Attribute) return;

	ResetCombo();
	StopAnimMontage();

	FVector DashDirection = GetActorForwardVector();

	// 1. FÍSICA: La aplicamos localmente al instante
	PerformDashLogic(DashDirection);

	// 2. VISUAL: Lo aplicamos localmente al instante (Predicción del Cliente)
	Local_DashFX(); 

	// 3. RED: Le avisamos al resto del mundo
	if (HasAuthority())
	{
		// Si soy el Host, yo mismo le aviso a todos los clientes
		Multicast_DashFX();
	}
	else
	{
		// Si soy un Cliente, le pido al Host que le avise a todos
		Server_Dash(DashDirection);
	}
}

void ANecroLifeCharacter::Server_Dash_Implementation(FVector DashDir)
{
    PerformDashLogic(DashDir);
    Multicast_DashFX();
}

void ANecroLifeCharacter::PerformDashLogic(FVector DashDir)
{
    bIsDashing = true;
    bCanDash = false;
    bIsInvincible = true;

    if (DashRootMotionID != 0)
    {
        GetCharacterMovement()->RemoveRootMotionSourceByID(DashRootMotionID);
    }

    TSharedPtr<FRootMotionSource_ConstantForce> DashForce = MakeShared<FRootMotionSource_ConstantForce>();
    DashForce->InstanceName = FName("DashForce");
    DashForce->AccumulateMode = ERootMotionAccumulateMode::Override;
    DashForce->Priority = 5;
    DashForce->Force = DashDir * Attribute->DashStrength;
    DashForce->Duration = Attribute->DashDuration;
    DashForce->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::SetVelocity;
    DashForce->FinishVelocityParams.SetVelocity = FVector::ZeroVector;

    DashRootMotionID = GetCharacterMovement()->ApplyRootMotionSource(DashForce);

    GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ANecroLifeCharacter::StopDash, Attribute->DashDuration, false);
}

void ANecroLifeCharacter::Local_DashFX()
{
    if (DashMontage)
    {
        PlayAnimMontage(DashMontage);
    }
    
    if (DashVFX)
    {
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
            DashVFX, GetMesh(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

        if (NiagaraComp) NiagaraComp->SetVariableObject(FName("User.SourceMesh"), GetMesh());
    }

    OriginalMaterials.Empty();
    if (TransparentMaterial)
    {
        for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
        {
            OriginalMaterials.Add(GetMesh()->GetMaterial(i));
            GetMesh()->SetMaterial(i, TransparentMaterial);
        }
    }

    if (DashSound) UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());

    OriginalWeaponMaterials.Empty();
    if (TransparentMaterial && WeaponMesh)
    {
        for (int32 i = 0; i < WeaponMesh->GetNumMaterials(); ++i)
        {
            OriginalWeaponMaterials.Add(WeaponMesh->GetMaterial(i));
            WeaponMesh->SetMaterial(i, TransparentMaterial);
        }
    }
}

void ANecroLifeCharacter::Multicast_DashFX_Implementation() 
{
	// Si NO soy el dueño de este personaje (soy un espectador en otra PC)
	if (!IsLocallyControlled())
	{
		Local_DashFX();
	}
}

void ANecroLifeCharacter::StopDash()
{
    bIsDashing = false;
    bIsInvincible = false;

    if (DashRootMotionID != 0)
    {
        GetCharacterMovement()->RemoveRootMotionSourceByID(DashRootMotionID);
        DashRootMotionID = 0;
    }

    for (int32 i = 0; i < OriginalMaterials.Num(); ++i)
    {
        if (OriginalMaterials[i]) GetMesh()->SetMaterial(i, OriginalMaterials[i]);
    }

    GetWorldTimerManager().SetTimer(CooldownTimerHandle, [this]()
        {
            bCanDash = true;
        }, Attribute->DashCooldown, false);

    if (WeaponMesh)
    {
        for (int32 i = 0; i < OriginalWeaponMaterials.Num(); ++i)
        {
            if (OriginalWeaponMaterials[i]) WeaponMesh->SetMaterial(i, OriginalWeaponMaterials[i]);
        }
    }
	
	// Si somos el servidor, obligamos a los espectadores a devolver los materiales
	if (HasAuthority())
	{
		Multicast_StopDashFX();
	}
}

/////////////////// INTERACCIÓN E INVENTARIO ///////////////////
void ANecroLifeCharacter::Interact()
{
   if (CurrentInteractable && CurrentInteractable->Implements<UNecroLifeInterface>())
   {
      INecroLifeInterface::Execute_OnInteract(CurrentInteractable, this);
   }
}

void ANecroLifeCharacter::InventoryInput()
{
   if (!IsLocallyControlled()) return;

   if (!bShowInventory)
   {
      bShowInventory = true;
      SetUIState(true);
      Attribute->RecalcularEstadisticas(Inventory->GemsInSlots);
      ShowInventory.Broadcast();
   }
   else
   {
      SetUIState(false);
      bShowInventory = false;
      ShowInventory.Broadcast();
   }
}

void ANecroLifeCharacter::TakePosion()
{
   if (MyPlayerState)
   {
      if (CachedInventoryComponent->UseHealtPosion())
      {
         HealthComponent->ApplyHealing(30.0f);
      }
   }
}

void ANecroLifeCharacter::AddCurrentQuest()
{
	if (ANecroLifeNpcBasic* NpcBasic = Cast<ANecroLifeNpcBasic>(CurrentInteractable))
	{
		QuestComponent->AddQuest(NpcBasic->QuestActual);
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
         return CurrentLine.bIsMissionChoice;
      }
   }
   return false;
}

/////////////////// GENERAL ///////////////////
void ANecroLifeCharacter::PossessedBy(AController* NewController)
{
   Super::PossessedBy(NewController);
   MyPlayerState = GetPlayerState<ANecroLifePlayerState>();
   if (MyPlayerState)
   {
      CachedAttributeComponent = MyPlayerState->GetAttributeComponent();
      CachedInventoryComponent = MyPlayerState->GetInventoryComponent();
      CachedQuestComponent = MyPlayerState->GetQuestComponent();
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
            if (InputMapping) Subsystem->AddMappingContext(InputMapping, 0);
         }
      }
   }

   if (Attribute)
   {
      Attribute->OnAtributosActualizados.AddDynamic(this, &ANecroLifeCharacter::OnAtributosActualizados);
      Attribute->RecalcularEstadisticas(Inventory->GemsInSlots);
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
   if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);
}

void ANecroLifeCharacter::OnAtributosActualizados(const FEstadisticasPersonaje& NuevosAtributos)
{
   GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? NuevosAtributos.Velocidad * 200.0f : NuevosAtributos.Velocidad * 50.0f;
}

void ANecroLifeCharacter::SetUIState(bool bIsTalking)
{
   APlayerController* PC = Cast<APlayerController>(GetController());
   if (PC)
   {
      if (bIsTalking)
      {
         PC->SetInputMode(FInputModeGameAndUI());
         PC->bShowMouseCursor = true;
      }
      else
      {
         PC->SetInputMode(FInputModeGameOnly());
         PC->bShowMouseCursor = false;
      }
   }
}

void ANecroLifeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
   if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
     
      EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
      EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
      EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Move);
      EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Look);

      EnhancedInputComponent->BindAction(MouseRightDown,ETriggerEvent::Triggered,this,&ANecroLifeCharacter::OnRightMouseDown);
      EnhancedInputComponent->BindAction(MouseRightUp,ETriggerEvent::Triggered,this,&ANecroLifeCharacter::OnRightMouseUp);
      EnhancedInputComponent->BindAction(MouseMiddleDown,ETriggerEvent::Triggered,this,&ANecroLifeCharacter::OnMiddleMouseDown);
      EnhancedInputComponent->BindAction(MouseMiddleUp,ETriggerEvent::Triggered,this,&ANecroLifeCharacter::OnMiddleMouseUp);

      EnhancedInputComponent->BindAction(CameraBoomAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::SetBoomLength);
      EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &ANecroLifeCharacter::AbilityEnabled);
      EnhancedInputComponent->BindAction(AbilityCancelAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::AbilityDisambled);
      EnhancedInputComponent->BindAction(OpenInventory, ETriggerEvent::Started, this, &ANecroLifeCharacter::InventoryInput);
      EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Dash);
      EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Interact);
      EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::RunActivated);
      EnhancedInputComponent->BindAction(ApplyPosion, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::TakePosion);
      EnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, this, &ANecroLifeCharacter::AplyAction);
   }
}

void ANecroLifeCharacter::Server_PlayCombatMontage_Implementation(UAnimMontage* MontageToPlay)
{
	// El servidor recibe el aviso y lo retransmite a todos
	Multicast_PlayCombatMontage(MontageToPlay);
}

void ANecroLifeCharacter::Multicast_PlayCombatMontage_Implementation(UAnimMontage* MontageToPlay)
{
	// Solo reproducimos en las pantallas de los ESPECTADORES.
	// El dueño original ya lo reprodujo al hacer clic, si no lo filtramos, lo vería doble.
	if (!IsLocallyControlled() && MontageToPlay)
	{
		PlayAnimMontage(MontageToPlay);
	}
}

void ANecroLifeCharacter::Multicast_StopDashFX_Implementation()
{
	// Los espectadores (Simulated Proxies) reciben el aviso de que el Dash terminó
	// y te devuelven los materiales originales.
	if (!IsLocallyControlled() && !HasAuthority())
	{
		for (int32 i = 0; i < OriginalMaterials.Num(); ++i)
		{
			if (OriginalMaterials[i]) GetMesh()->SetMaterial(i, OriginalMaterials[i]);
		}
		if (WeaponMesh)
		{
			for (int32 i = 0; i < OriginalWeaponMaterials.Num(); ++i)
			{
				if (OriginalWeaponMaterials[i]) WeaponMesh->SetMaterial(i, OriginalWeaponMaterials[i]);
			}
		}
	}
}