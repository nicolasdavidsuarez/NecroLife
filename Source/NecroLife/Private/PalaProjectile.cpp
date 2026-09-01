#include "PalaProjectile.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NPC/NecroLifeEnemyBasic.h"
#include "Public/Components/RPGHelper.h"

// ============================================================
// Constructor
// ============================================================
APalaProjectile::APalaProjectile()
{
    PrimaryActorTick.bCanEverTick = true; // AHORA ES TRUE

    // 1. Colisión Principal (Caja)
    CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    
    // Le damos un tamaño inicial genérico (X=Largo, Y=Ancho, Z=Alto)
    CollisionComp->InitBoxExtent(FVector(40.0f, 40.0f, 40.0f)); 
    CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = CollisionComp;

    PalaMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PalaMesh"));
    PalaMesh->SetupAttachment(RootComponent);
    
    // Destrucción automática a los 3 segundos para limpiar la memoria si no choca con nada
    InitialLifeSpan = 3.0f;
    
    // --- Configuración del Movimiento ---
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    
    // Le decimos que el componente que manda en las físicas es la esfera
    ProjectileMovement->UpdatedComponent = CollisionComp;
    
    // Velocidad por defecto (podés cambiarla en los Blueprints)
    ProjectileMovement->InitialSpeed = 2000.0f;
    ProjectileMovement->MaxSpeed = 2000.0f;
    
    // TRUCO CLAVE: Esto hace que la punta de la lanza mire hacia donde viaja (ideal para el SpreadAngle)
    ProjectileMovement->bRotationFollowsVelocity = true; 
    
    // Desactivamos la gravedad para que viajen en línea recta como un láser
    ProjectileMovement->ProjectileGravityScale = 0.0f;
}

// ============================================================
// Begin Play
// ============================================================
void APalaProjectile::BeginPlay()
{
    Super::BeginPlay();
    
    // Guardamos la dirección en la que fue apuntado al nacer
    InitialForwardDir = GetActorForwardVector();
    InitialRightDir = GetActorRightVector();
    
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APalaProjectile::OnProjectileOverlap);
}

// ============================================================
// Lógica de Impacto
// ============================================================
void APalaProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;

    if (OtherActor && OtherActor != this && OtherActor != GetInstigator() && !DamagedActors.Contains(OtherActor))
    {
        if (ANecroLifeEnemyBasic* Enemy = Cast<ANecroLifeEnemyBasic>(OtherActor))
        {
            DamagedActors.Add(OtherActor);
            URPGHelper::ApplyDamage(Enemy, DamageAmount);
            Enemy->IsAlive(); // dispara Die() si la salud llegó a 0
        }
    }
}

void APalaProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Solo afectamos el vuelo si marcaste el Checkbox en el Blueprint
    if (bIsSinusoidal && ProjectileMovement)
    {
        float Time = GetGameTimeSinceCreation();
        
        // MATEMÁTICA DE JUEGOS: Para que la trayectoria trace un Seno perfecto, 
        // le aplicamos la función Coseno a su Velocidad Lateral (la derivada).
        float LateralVelocity = FMath::Cos(Time * SineFrequency) * SineAmplitude;
        
        // Combinamos la velocidad hacia adelante base con el zig-zag lateral
        FVector NewVelocity = (InitialForwardDir * ProjectileMovement->InitialSpeed) + (InitialRightDir * LateralVelocity);
        
        // Sobrescribimos la velocidad del motor
        ProjectileMovement->Velocity = NewVelocity;
    }
}

