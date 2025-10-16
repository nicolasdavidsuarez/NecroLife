// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Menu.h"

#ifdef MULTIPLAYERSESSIONOW_Menu_generated_h
#error "Menu.generated.h already included, missing '#pragma once' in Menu.h"
#endif
#define MULTIPLAYERSESSIONOW_Menu_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class UMenu ********************************************************************
#define FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execBotonUnirseClickeado); \
	DECLARE_FUNCTION(execBotonHostClickeado); \
	DECLARE_FUNCTION(execAlIniciarSesion); \
	DECLARE_FUNCTION(execAlDestruirSesion); \
	DECLARE_FUNCTION(execAlCrearSesion); \
	DECLARE_FUNCTION(execConfigurarMenu);


MULTIPLAYERSESSIONOW_API UClass* Z_Construct_UClass_UMenu_NoRegister();

#define FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h_13_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUMenu(); \
	friend struct Z_Construct_UClass_UMenu_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend MULTIPLAYERSESSIONOW_API UClass* Z_Construct_UClass_UMenu_NoRegister(); \
public: \
	DECLARE_CLASS2(UMenu, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/MultiplayerSessionOW"), Z_Construct_UClass_UMenu_NoRegister) \
	DECLARE_SERIALIZER(UMenu)


#define FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h_13_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UMenu(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UMenu(UMenu&&) = delete; \
	UMenu(const UMenu&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UMenu); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UMenu); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UMenu) \
	NO_API virtual ~UMenu();


#define FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h_10_PROLOG
#define FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h_13_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h_13_INCLASS_NO_PURE_DECLS \
	FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h_13_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UMenu;

// ********** End Class UMenu **********************************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
