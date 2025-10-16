// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "MultiplayerSessionsSubsystem.h"

#ifdef MULTIPLAYERSESSIONOW_MultiplayerSessionsSubsystem_generated_h
#error "MultiplayerSessionsSubsystem.generated.h already included, missing '#pragma once' in MultiplayerSessionsSubsystem.h"
#endif
#define MULTIPLAYERSESSIONOW_MultiplayerSessionsSubsystem_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Delegate FMultijugadorAlCrearSesionCompletada **********************************
#define FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h_12_DELEGATE \
MULTIPLAYERSESSIONOW_API void FMultijugadorAlCrearSesionCompletada_DelegateWrapper(const FMulticastScriptDelegate& MultijugadorAlCrearSesionCompletada, bool bFueExitoso);


// ********** End Delegate FMultijugadorAlCrearSesionCompletada ************************************

// ********** Begin Delegate FMultijugadorAlDestruirSesionCompletada *******************************
#define FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h_15_DELEGATE \
MULTIPLAYERSESSIONOW_API void FMultijugadorAlDestruirSesionCompletada_DelegateWrapper(const FMulticastScriptDelegate& MultijugadorAlDestruirSesionCompletada, bool bFueExitoso);


// ********** End Delegate FMultijugadorAlDestruirSesionCompletada *********************************

// ********** Begin Delegate FMultijugadorAlIniciarSesionCompletada ********************************
#define FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h_16_DELEGATE \
MULTIPLAYERSESSIONOW_API void FMultijugadorAlIniciarSesionCompletada_DelegateWrapper(const FMulticastScriptDelegate& MultijugadorAlIniciarSesionCompletada, bool bFueExitoso);


// ********** End Delegate FMultijugadorAlIniciarSesionCompletada **********************************

// ********** Begin Class UMultiplayerSessionsSubsystem ********************************************
MULTIPLAYERSESSIONOW_API UClass* Z_Construct_UClass_UMultiplayerSessionsSubsystem_NoRegister();

#define FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h_24_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUMultiplayerSessionsSubsystem(); \
	friend struct Z_Construct_UClass_UMultiplayerSessionsSubsystem_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend MULTIPLAYERSESSIONOW_API UClass* Z_Construct_UClass_UMultiplayerSessionsSubsystem_NoRegister(); \
public: \
	DECLARE_CLASS2(UMultiplayerSessionsSubsystem, UGameInstanceSubsystem, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/MultiplayerSessionOW"), Z_Construct_UClass_UMultiplayerSessionsSubsystem_NoRegister) \
	DECLARE_SERIALIZER(UMultiplayerSessionsSubsystem)


#define FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h_24_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	UMultiplayerSessionsSubsystem(UMultiplayerSessionsSubsystem&&) = delete; \
	UMultiplayerSessionsSubsystem(const UMultiplayerSessionsSubsystem&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UMultiplayerSessionsSubsystem); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UMultiplayerSessionsSubsystem); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UMultiplayerSessionsSubsystem) \
	NO_API virtual ~UMultiplayerSessionsSubsystem();


#define FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h_21_PROLOG
#define FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h_24_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h_24_INCLASS_NO_PURE_DECLS \
	FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h_24_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UMultiplayerSessionsSubsystem;

// ********** End Class UMultiplayerSessionsSubsystem **********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
