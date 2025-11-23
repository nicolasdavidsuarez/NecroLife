// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/AdvancedPlatform.h"




// Sets default values
AAdvancedPlatform::AAdvancedPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	PlatformMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Mesh"));
	PlatformDoors=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Doors"));
	RootComponent=PlatformMesh;
	PlatformDoors->SetupAttachment(PlatformMesh);
	SphereCollision->SetupAttachment(PlatformMesh);
	bReplicates=true;
    SetReplicatingMovement(true);
	CurrentOpacity=0.f;
	State=EPlatformState::Visible;
	Material=PlatformMesh->CreateAndSetMaterialInstanceDynamic(0);
	// SIEMPRE verifica que funcionó antes de usarlo
	if (Material)
	{
		UE_LOG(LogTemp, Log, TEXT("¡Material Dinámico creado con éxito!"));
		// Aquí puedes poner un valor inicial si quieres
		Material->SetScalarParameterValue(FName("Opacity"), 0.5f);
	}
	
} 

// Called when the game starts or when spawned
void AAdvancedPlatform::BeginPlay()
{
	Super::BeginPlay();
	Material=PlatformMesh->CreateAndSetMaterialInstanceDynamic(0);
	// SIEMPRE verifica que funcionó antes de usarlo
	if (Material)
	{
		UE_LOG(LogTemp, Log, TEXT("¡Material Dinámico creado con éxito!"));
		// Aquí puedes poner un valor inicial si quieres
		Material->SetScalarParameterValue(FName("Opacity"), 1.0f);
	}
	UpdateOpacity();
	if (State==EPlatformState::Hidden)
	{
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
	}
	State=EPlatformState::FadingIn;
}


// Called every frame
void AAdvancedPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
	case EPlatformState::FadingIn:
		{
			// Sumar opacidad
			CurrentOpacity += DeltaTime * FadeDuration;
            
			// Si llegamos a 1, cambiamos a estado Visible
			if (CurrentOpacity >= 1.0f)
			{
				CurrentOpacity = 1.0f;
				State = EPlatformState::Visible;
                
				
				SetActorTickEnabled(false); 
			}
			UpdateOpacity();
			break;
		}
	case EPlatformState::FadingOut:
		{
			// Restar opacidad
			CurrentOpacity -= DeltaTime * FadeDuration;
            
			// Si llegamos a 0, cambiamos a estado Hidden
			if (CurrentOpacity <= 0.0f)
			{
				CurrentOpacity = 0.0f;
				State = EPlatformState::Hidden;
                
				// Apagamos colisión para que el jugador no choque con el aire
				SetActorEnableCollision(false);
                
				// YA NO NECESITAMOS TICK
				SetActorTickEnabled(false);
				// Y Hasta ahora solo me hacen falta una vez las plataformas
				Destroy();
			}
			UpdateOpacity();
						break;
		}
          default:
		break;
	}
	
	
	if (Path.Num()>0&&bIsOverlappingPlatform)
	{
		if (Path.Num()==Nodo)
		{
			State=EPlatformState::FadingOut;
		}else{
		FVector Current = GetActorLocation();
		FVector Target = Path[Nodo];

		FVector NewLocation = FMath::VInterpConstantTo(Current, Target, DeltaTime, Speed);
		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, Target) < 2.f&&State!=EPlatformState::FadingOut)
		{
	     Nodo++;
		}
		}	
	}
}



void AAdvancedPlatform::NotifyActorBeginOverlap(AActor* OtherActor)
{
	SetActorTickEnabled(true);
	Super::NotifyActorBeginOverlap(OtherActor);
	bIsOverlappingPlatform=true;
	PlatformDoors->SetRelativeLocation(PlatformClose);
	
}

void AAdvancedPlatform::FadeIn()
{
	// Solo iniciamos si no está ya visible
	if (State != EPlatformState::Visible && State != EPlatformState::FadingIn)
	{
		 State = EPlatformState::FadingIn;
        
		// IMPORTANTE: Activar visuales, colisión y Tick
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		SetActorTickEnabled(true);
	}
}

void AAdvancedPlatform::FadeOut()
{
	// Solo iniciamos si no está ya oculto
	if (State != EPlatformState::Hidden && State != EPlatformState::FadingOut)
	{
		State = EPlatformState::FadingOut;
        
		// Necesitamos Tick para animar
		SetActorTickEnabled(true);
	}
}

void AAdvancedPlatform::UpdateOpacity()
{
	if (Material)
	{
		// "Opacity" debe ser el nombre exacto de tu Scalar Parameter en el Material
		Material->SetScalarParameterValue(FName("Opacity"), CurrentOpacity);
	}
}

