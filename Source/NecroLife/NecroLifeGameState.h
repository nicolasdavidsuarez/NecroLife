// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NecroLifeGameState.generated.h"

class UQuestComponent;
/**
 * 
 */
/**
 * [PARA TODO EL EQUIPO]: 
 * El GameState es el "Pizarrón Público" de la partida. 
 * A diferencia del Player (que es privado de cada uno), esta clase se copia automáticamente 
 * a través de internet a todos los jugadores conectados. 
 * Si ponemos algo aquí, todos lo pueden ver.
 */
UCLASS()
class NECROLIFE_API ANecroLifeGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// Constructor: Es lo primero que se ejecuta cuando arranca el mapa.
	ANecroLifeGameState();

	// [PARA DISEÑADORES Y PRODUCTORES]: 
	// Este es nuestro "Director de Misiones". Lo ponemos aquí para que el modo cooperativo funcione.
	// Si la misión dice "Matar 10 monstruos", este componente lleva la cuenta global.
	// No importa quién mate al monstruo, la cuenta sube para todos.
	//
	// [PARA ARTISTAS 2D / UI]:
	// La interfaz gráfica (los widgets en pantalla) van a buscar la información a esta variable
	// para saber qué texto e ícono de misión tienen que dibujar.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Misiones Cooperativas")
	UQuestComponent* QuestComponent;
};