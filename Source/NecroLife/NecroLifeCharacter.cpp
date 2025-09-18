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
#include "Components/Public/AttributeComponent.h"

ANecroLifeCharacter::ANecroLifeCharacter()
{
	// crear y atachar el UHealthComponent
	HealthComponent = CreateDefaultSubobject<UUHealthComponent>(TEXT("HealthComponent"));
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
	
	if (InputVector.X>0&&armLength<1200)
	{
		armLength += CameraBoom->TargetArmLength*0.05f;
		CameraBoom->TargetArmLength = armLength;
	}
	else
	{
		armLength -= CameraBoom->TargetArmLength*0.05f;
		CameraBoom->TargetArmLength = armLength;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ANecroLifeCharacter::AbilityEnabled(const FInputActionValue& InputActionValue)
{
	int32 pressedKeys = static_cast<int32>(InputActionValue.Get<float>());
	FString AbilityName =FString("se entra en modo combate " + FString::FromInt(pressedKeys));
	ShowMsg(AbilityName);
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
			DrawDebugLine(GetWorld(),GetActorLocation(),TargetLocation,FColor::Emerald,
				false,3.0f,1,10);
		}
	
		bEnabledAbility = true;		
	}
	
}

void ANecroLifeCharacter::AbilityDisambled(const FInputActionValue& InputActionValue)
{
	ShowMsg(FString::Printf(TEXT("Se Cancela Habilidad")));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bEnabledAbility = false;
}

void ANecroLifeCharacter::AplyAction()
{
	if (bEnabledAbility)
	{
		ShowMsg(FString::Printf(TEXT("Ability Ejecuted")));
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
		bEnabledAbility = false;
	}else
	{
		ShowMsg(FString::Printf(TEXT("Se ejecuta Ataque Melee")));
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
		EnhancedInputComponent->BindAction(CameraBoomAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::SetBoomLength);
		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::AbilityEnabled);
		EnhancedInputComponent->BindAction(AbilityCancelAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::AbilityDisambled);
		///Dash//////////////////
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Dash);
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
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ANecroLifeCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 45);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
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
		DrawDebugCone(GetWorld(),PlayerPos,ToMouse,450.0f,0.5,0.5,12,FColor::Blue,false,0.1f);
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

