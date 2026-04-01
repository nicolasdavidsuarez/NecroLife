// Fill out your copyright notice in the Description page of Project Settings.


#include "NecroLifeGameState.h"
// [PARA PROGRAMADORES]: Ahora sí incluimos el archivo real de nuestro componente para poder usarlo.
#include "Components/QuestComponent.h"

ANecroLifeGameState::ANecroLifeGameState()
{
	// [PARA PRODUCTORES Y DISEÑADORES]: 
	// Aquí le decimos al motor: "Apenas arranque la partida, creá y enganchá un administrador de misiones al nivel".
	// CreateDefaultSubobject es la función de C++ que "fabrica" el componente.
	QuestManager = CreateDefaultSubobject<UQuestComponent>(TEXT("GestorDeMisionesCooperativo"));

	// [PARA ARTISTAS 3D]: 
	// Ojo, este componente es 100% lógica invisible. No tiene malla 3D (Static Mesh), 
	// ni animaciones, ni colisiones en el mundo. Es código puro corriendo en el fondo.
	
	// [PARA PROGRAMADORES]:
	// Nos aseguramos de que este GameState pueda enviar información por la red.
	bReplicates = true; 
}