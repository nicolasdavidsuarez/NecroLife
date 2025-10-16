// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Menu.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeMenu() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
MULTIPLAYERSESSIONOW_API UClass* Z_Construct_UClass_UMenu();
MULTIPLAYERSESSIONOW_API UClass* Z_Construct_UClass_UMenu_NoRegister();
MULTIPLAYERSESSIONOW_API UClass* Z_Construct_UClass_UWidgetSessionSerch_NoRegister();
UMG_API UClass* Z_Construct_UClass_UButton_NoRegister();
UMG_API UClass* Z_Construct_UClass_UEditableText_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UMG_API UClass* Z_Construct_UClass_UVerticalBox_NoRegister();
UPackage* Z_Construct_UPackage__Script_MultiplayerSessionOW();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UMenu Function AlCrearSesion *********************************************
struct Z_Construct_UFunction_UMenu_AlCrearSesion_Statics
{
	struct Menu_eventAlCrearSesion_Parms
	{
		bool bFueExitoso;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Callbacks para los delegados del SubSistema de Sesiones\n" },
#endif
		{ "ModuleRelativePath", "Public/Menu.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Callbacks para los delegados del SubSistema de Sesiones" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_bFueExitoso_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bFueExitoso;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::NewProp_bFueExitoso_SetBit(void* Obj)
{
	((Menu_eventAlCrearSesion_Parms*)Obj)->bFueExitoso = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::NewProp_bFueExitoso = { "bFueExitoso", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Menu_eventAlCrearSesion_Parms), &Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::NewProp_bFueExitoso_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::NewProp_bFueExitoso,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "AlCrearSesion", Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::Menu_eventAlCrearSesion_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::Menu_eventAlCrearSesion_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_AlCrearSesion()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_AlCrearSesion_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execAlCrearSesion)
{
	P_GET_UBOOL(Z_Param_bFueExitoso);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AlCrearSesion(Z_Param_bFueExitoso);
	P_NATIVE_END;
}
// ********** End Class UMenu Function AlCrearSesion ***********************************************

// ********** Begin Class UMenu Function AlDestruirSesion ******************************************
struct Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics
{
	struct Menu_eventAlDestruirSesion_Parms
	{
		bool bFueExitoso;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bFueExitoso_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bFueExitoso;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::NewProp_bFueExitoso_SetBit(void* Obj)
{
	((Menu_eventAlDestruirSesion_Parms*)Obj)->bFueExitoso = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::NewProp_bFueExitoso = { "bFueExitoso", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Menu_eventAlDestruirSesion_Parms), &Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::NewProp_bFueExitoso_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::NewProp_bFueExitoso,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "AlDestruirSesion", Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::Menu_eventAlDestruirSesion_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::Menu_eventAlDestruirSesion_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_AlDestruirSesion()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_AlDestruirSesion_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execAlDestruirSesion)
{
	P_GET_UBOOL(Z_Param_bFueExitoso);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AlDestruirSesion(Z_Param_bFueExitoso);
	P_NATIVE_END;
}
// ********** End Class UMenu Function AlDestruirSesion ********************************************

// ********** Begin Class UMenu Function AlIniciarSesion *******************************************
struct Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics
{
	struct Menu_eventAlIniciarSesion_Parms
	{
		bool bFueExitoso;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bFueExitoso_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bFueExitoso;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::NewProp_bFueExitoso_SetBit(void* Obj)
{
	((Menu_eventAlIniciarSesion_Parms*)Obj)->bFueExitoso = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::NewProp_bFueExitoso = { "bFueExitoso", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Menu_eventAlIniciarSesion_Parms), &Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::NewProp_bFueExitoso_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::NewProp_bFueExitoso,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "AlIniciarSesion", Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::Menu_eventAlIniciarSesion_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::Menu_eventAlIniciarSesion_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_AlIniciarSesion()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_AlIniciarSesion_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execAlIniciarSesion)
{
	P_GET_UBOOL(Z_Param_bFueExitoso);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->AlIniciarSesion(Z_Param_bFueExitoso);
	P_NATIVE_END;
}
// ********** End Class UMenu Function AlIniciarSesion *********************************************

// ********** Begin Class UMenu Function BotonHostClickeado ****************************************
struct Z_Construct_UFunction_UMenu_BotonHostClickeado_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_BotonHostClickeado_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "BotonHostClickeado", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_BotonHostClickeado_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_BotonHostClickeado_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UMenu_BotonHostClickeado()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_BotonHostClickeado_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execBotonHostClickeado)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->BotonHostClickeado();
	P_NATIVE_END;
}
// ********** End Class UMenu Function BotonHostClickeado ******************************************

// ********** Begin Class UMenu Function BotonUnirseClickeado **************************************
struct Z_Construct_UFunction_UMenu_BotonUnirseClickeado_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_BotonUnirseClickeado_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "BotonUnirseClickeado", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_BotonUnirseClickeado_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_BotonUnirseClickeado_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UMenu_BotonUnirseClickeado()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_BotonUnirseClickeado_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execBotonUnirseClickeado)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->BotonUnirseClickeado();
	P_NATIVE_END;
}
// ********** End Class UMenu Function BotonUnirseClickeado ****************************************

// ********** Begin Class UMenu Function ConfigurarMenu ********************************************
struct Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics
{
	struct Menu_eventConfigurarMenu_Parms
	{
		int32 NumeroConexionesPublicas;
		FString TipoDePartida;
		FString RutaLobby;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "CPP_Default_NumeroConexionesPublicas", "4" },
		{ "CPP_Default_RutaLobby", "/Game/ThirdPersonCPP/Maps/Lobby" },
		{ "CPP_Default_TipoDePartida", "KenoTeach" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_NumeroConexionesPublicas;
	static const UECodeGen_Private::FStrPropertyParams NewProp_TipoDePartida;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RutaLobby;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::NewProp_NumeroConexionesPublicas = { "NumeroConexionesPublicas", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventConfigurarMenu_Parms, NumeroConexionesPublicas), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::NewProp_TipoDePartida = { "TipoDePartida", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventConfigurarMenu_Parms, TipoDePartida), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::NewProp_RutaLobby = { "RutaLobby", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Menu_eventConfigurarMenu_Parms, RutaLobby), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::NewProp_NumeroConexionesPublicas,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::NewProp_TipoDePartida,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::NewProp_RutaLobby,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UMenu, nullptr, "ConfigurarMenu", Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::PropPointers), sizeof(Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::Menu_eventConfigurarMenu_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::Menu_eventConfigurarMenu_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMenu_ConfigurarMenu()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMenu_ConfigurarMenu_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMenu::execConfigurarMenu)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_NumeroConexionesPublicas);
	P_GET_PROPERTY(FStrProperty,Z_Param_TipoDePartida);
	P_GET_PROPERTY(FStrProperty,Z_Param_RutaLobby);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ConfigurarMenu(Z_Param_NumeroConexionesPublicas,Z_Param_TipoDePartida,Z_Param_RutaLobby);
	P_NATIVE_END;
}
// ********** End Class UMenu Function ConfigurarMenu **********************************************

// ********** Begin Class UMenu ********************************************************************
void UMenu::StaticRegisterNativesUMenu()
{
	UClass* Class = UMenu::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "AlCrearSesion", &UMenu::execAlCrearSesion },
		{ "AlDestruirSesion", &UMenu::execAlDestruirSesion },
		{ "AlIniciarSesion", &UMenu::execAlIniciarSesion },
		{ "BotonHostClickeado", &UMenu::execBotonHostClickeado },
		{ "BotonUnirseClickeado", &UMenu::execBotonUnirseClickeado },
		{ "ConfigurarMenu", &UMenu::execConfigurarMenu },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_UMenu;
UClass* UMenu::GetPrivateStaticClass()
{
	using TClass = UMenu;
	if (!Z_Registration_Info_UClass_UMenu.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("Menu"),
			Z_Registration_Info_UClass_UMenu.InnerSingleton,
			StaticRegisterNativesUMenu,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_UMenu.InnerSingleton;
}
UClass* Z_Construct_UClass_UMenu_NoRegister()
{
	return UMenu::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UMenu_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Menu.h" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OpcionSesion_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ClaseWidgetBusqueda_MetaData[] = {
		{ "Category", "Busqueda de sesiones" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// SUGERIDO: usar la clase concreta del widget de item de busqueda\n" },
#endif
		{ "ModuleRelativePath", "Public/Menu.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "SUGERIDO: usar la clase concreta del widget de item de busqueda" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BotonHost_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BotonUnirse_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CajaResultados_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TextoNombreHost_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Menu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_OpcionSesion;
	static const UECodeGen_Private::FClassPropertyParams NewProp_ClaseWidgetBusqueda;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_BotonHost;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_BotonUnirse;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CajaResultados;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TextoNombreHost;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UMenu_AlCrearSesion, "AlCrearSesion" }, // 204398045
		{ &Z_Construct_UFunction_UMenu_AlDestruirSesion, "AlDestruirSesion" }, // 475039498
		{ &Z_Construct_UFunction_UMenu_AlIniciarSesion, "AlIniciarSesion" }, // 4285328509
		{ &Z_Construct_UFunction_UMenu_BotonHostClickeado, "BotonHostClickeado" }, // 3117941951
		{ &Z_Construct_UFunction_UMenu_BotonUnirseClickeado, "BotonUnirseClickeado" }, // 2364200942
		{ &Z_Construct_UFunction_UMenu_ConfigurarMenu, "ConfigurarMenu" }, // 614490964
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMenu>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_OpcionSesion = { "OpcionSesion", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, OpcionSesion), Z_Construct_UClass_UWidgetSessionSerch_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OpcionSesion_MetaData), NewProp_OpcionSesion_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_ClaseWidgetBusqueda = { "ClaseWidgetBusqueda", nullptr, (EPropertyFlags)0x0014000000000001, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, ClaseWidgetBusqueda), Z_Construct_UClass_UClass, Z_Construct_UClass_UWidgetSessionSerch_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ClaseWidgetBusqueda_MetaData), NewProp_ClaseWidgetBusqueda_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_BotonHost = { "BotonHost", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, BotonHost), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BotonHost_MetaData), NewProp_BotonHost_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_BotonUnirse = { "BotonUnirse", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, BotonUnirse), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BotonUnirse_MetaData), NewProp_BotonUnirse_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_CajaResultados = { "CajaResultados", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, CajaResultados), Z_Construct_UClass_UVerticalBox_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CajaResultados_MetaData), NewProp_CajaResultados_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMenu_Statics::NewProp_TextoNombreHost = { "TextoNombreHost", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMenu, TextoNombreHost), Z_Construct_UClass_UEditableText_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TextoNombreHost_MetaData), NewProp_TextoNombreHost_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UMenu_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_OpcionSesion,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_ClaseWidgetBusqueda,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_BotonHost,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_BotonUnirse,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_CajaResultados,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMenu_Statics::NewProp_TextoNombreHost,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMenu_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UMenu_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSessionOW,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMenu_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UMenu_Statics::ClassParams = {
	&UMenu::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UMenu_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UMenu_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMenu_Statics::Class_MetaDataParams), Z_Construct_UClass_UMenu_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UMenu()
{
	if (!Z_Registration_Info_UClass_UMenu.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMenu.OuterSingleton, Z_Construct_UClass_UMenu_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UMenu.OuterSingleton;
}
UMenu::UMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UMenu);
UMenu::~UMenu() {}
// ********** End Class UMenu **********************************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h__Script_MultiplayerSessionOW_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UMenu, UMenu::StaticClass, TEXT("UMenu"), &Z_Registration_Info_UClass_UMenu, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMenu), 3112400758U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h__Script_MultiplayerSessionOW_3988315995(TEXT("/Script/MultiplayerSessionOW"),
	Z_CompiledInDeferFile_FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h__Script_MultiplayerSessionOW_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_Menu_h__Script_MultiplayerSessionOW_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
