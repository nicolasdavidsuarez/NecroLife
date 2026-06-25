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
    PrimaryActorTick.bCanEverTick = false; // No necesitamos Tick, optimizamos rendimiento

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
    
    // Conectamos el evento de impacto
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APalaProjectile::OnProjectileOverlap);
}

// ============================================================
// Lógica de Impacto
// ============================================================
void APalaProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Verificamos que toquemos algo válido, que no seamos nosotros mismos, que no sea Huesos (el instigador) y que no lo hayamos golpeado ya
    if (OtherActor && OtherActor != this && OtherActor != GetInstigator() && !DamagedActors.Contains(OtherActor))
    {
        // Si el objeto tocado es un enemigo...
        if (ANecroLifeEnemyBasic* Enemy = Cast<ANecroLifeEnemyBasic>(OtherActor))
        {
            // Lo guardamos en memoria para atravesarlo y no volver a dañarlo
            DamagedActors.Add(OtherActor);

            // Aplicamos el daño
            URPGHelper::ApplyDamage(Enemy, DamageAmount);
        }
    }
}

