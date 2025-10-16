// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Engine/GameInstance.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeMultiplayerSessionsSubsystem() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_UGameInstanceSubsystem();
MULTIPLAYERSESSIONOW_API UClass* Z_Construct_UClass_UMultiplayerSessionsSubsystem();
MULTIPLAYERSESSIONOW_API UClass* Z_Construct_UClass_UMultiplayerSessionsSubsystem_NoRegister();
MULTIPLAYERSESSIONOW_API UFunction* Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature();
MULTIPLAYERSESSIONOW_API UFunction* Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature();
MULTIPLAYERSESSIONOW_API UFunction* Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature();
UPackage* Z_Construct_UPackage__Script_MultiplayerSessionOW();
// ********** End Cross Module References **********************************************************

// ********** Begin Delegate FMultijugadorAlCrearSesionCompletada **********************************
struct Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics
{
	struct _Script_MultiplayerSessionOW_eventMultijugadorAlCrearSesionCompletada_Parms
	{
		bool bFueExitoso;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "//\n// Declaramos nuestros propios delegados para que Menu pueda enlazar callbacks\n//\n" },
#endif
		{ "ModuleRelativePath", "Public/MultiplayerSessionsSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Declaramos nuestros propios delegados para que Menu pueda enlazar callbacks" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_bFueExitoso_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bFueExitoso;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
void Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::NewProp_bFueExitoso_SetBit(void* Obj)
{
	((_Script_MultiplayerSessionOW_eventMultijugadorAlCrearSesionCompletada_Parms*)Obj)->bFueExitoso = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::NewProp_bFueExitoso = { "bFueExitoso", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(_Script_MultiplayerSessionOW_eventMultijugadorAlCrearSesionCompletada_Parms), &Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::NewProp_bFueExitoso_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::NewProp_bFueExitoso,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_MultiplayerSessionOW, nullptr, "MultijugadorAlCrearSesionCompletada__DelegateSignature", Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::_Script_MultiplayerSessionOW_eventMultijugadorAlCrearSesionCompletada_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::_Script_MultiplayerSessionOW_eventMultijugadorAlCrearSesionCompletada_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FMultijugadorAlCrearSesionCompletada_DelegateWrapper(const FMulticastScriptDelegate& MultijugadorAlCrearSesionCompletada, bool bFueExitoso)
{
	struct _Script_MultiplayerSessionOW_eventMultijugadorAlCrearSesionCompletada_Parms
	{
		bool bFueExitoso;
	};
	_Script_MultiplayerSessionOW_eventMultijugadorAlCrearSesionCompletada_Parms Parms;
	Parms.bFueExitoso=bFueExitoso ? true : false;
	MultijugadorAlCrearSesionCompletada.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FMultijugadorAlCrearSesionCompletada ************************************

// ********** Begin Delegate FMultijugadorAlDestruirSesionCompletada *******************************
struct Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics
{
	struct _Script_MultiplayerSessionOW_eventMultijugadorAlDestruirSesionCompletada_Parms
	{
		bool bFueExitoso;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/MultiplayerSessionsSubsystem.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bFueExitoso_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bFueExitoso;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
void Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::NewProp_bFueExitoso_SetBit(void* Obj)
{
	((_Script_MultiplayerSessionOW_eventMultijugadorAlDestruirSesionCompletada_Parms*)Obj)->bFueExitoso = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::NewProp_bFueExitoso = { "bFueExitoso", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(_Script_MultiplayerSessionOW_eventMultijugadorAlDestruirSesionCompletada_Parms), &Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::NewProp_bFueExitoso_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::NewProp_bFueExitoso,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_MultiplayerSessionOW, nullptr, "MultijugadorAlDestruirSesionCompletada__DelegateSignature", Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::_Script_MultiplayerSessionOW_eventMultijugadorAlDestruirSesionCompletada_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::_Script_MultiplayerSessionOW_eventMultijugadorAlDestruirSesionCompletada_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FMultijugadorAlDestruirSesionCompletada_DelegateWrapper(const FMulticastScriptDelegate& MultijugadorAlDestruirSesionCompletada, bool bFueExitoso)
{
	struct _Script_MultiplayerSessionOW_eventMultijugadorAlDestruirSesionCompletada_Parms
	{
		bool bFueExitoso;
	};
	_Script_MultiplayerSessionOW_eventMultijugadorAlDestruirSesionCompletada_Parms Parms;
	Parms.bFueExitoso=bFueExitoso ? true : false;
	MultijugadorAlDestruirSesionCompletada.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FMultijugadorAlDestruirSesionCompletada *********************************

// ********** Begin Delegate FMultijugadorAlIniciarSesionCompletada ********************************
struct Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics
{
	struct _Script_MultiplayerSessionOW_eventMultijugadorAlIniciarSesionCompletada_Parms
	{
		bool bFueExitoso;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/MultiplayerSessionsSubsystem.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bFueExitoso_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bFueExitoso;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
void Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::NewProp_bFueExitoso_SetBit(void* Obj)
{
	((_Script_MultiplayerSessionOW_eventMultijugadorAlIniciarSesionCompletada_Parms*)Obj)->bFueExitoso = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::NewProp_bFueExitoso = { "bFueExitoso", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(_Script_MultiplayerSessionOW_eventMultijugadorAlIniciarSesionCompletada_Parms), &Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::NewProp_bFueExitoso_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::NewProp_bFueExitoso,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_MultiplayerSessionOW, nullptr, "MultijugadorAlIniciarSesionCompletada__DelegateSignature", Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::_Script_MultiplayerSessionOW_eventMultijugadorAlIniciarSesionCompletada_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::_Script_MultiplayerSessionOW_eventMultijugadorAlIniciarSesionCompletada_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FMultijugadorAlIniciarSesionCompletada_DelegateWrapper(const FMulticastScriptDelegate& MultijugadorAlIniciarSesionCompletada, bool bFueExitoso)
{
	struct _Script_MultiplayerSessionOW_eventMultijugadorAlIniciarSesionCompletada_Parms
	{
		bool bFueExitoso;
	};
	_Script_MultiplayerSessionOW_eventMultijugadorAlIniciarSesionCompletada_Parms Parms;
	Parms.bFueExitoso=bFueExitoso ? true : false;
	MultijugadorAlIniciarSesionCompletada.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FMultijugadorAlIniciarSesionCompletada **********************************

// ********** Begin Class UMultiplayerSessionsSubsystem ********************************************
void UMultiplayerSessionsSubsystem::StaticRegisterNativesUMultiplayerSessionsSubsystem()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_UMultiplayerSessionsSubsystem;
UClass* UMultiplayerSessionsSubsystem::GetPrivateStaticClass()
{
	using TClass = UMultiplayerSessionsSubsystem;
	if (!Z_Registration_Info_UClass_UMultiplayerSessionsSubsystem.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("MultiplayerSessionsSubsystem"),
			Z_Registration_Info_UClass_UMultiplayerSessionsSubsystem.InnerSingleton,
			StaticRegisterNativesUMultiplayerSessionsSubsystem,
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
	return Z_Registration_Info_UClass_UMultiplayerSessionsSubsystem.InnerSingleton;
}
UClass* Z_Construct_UClass_UMultiplayerSessionsSubsystem_NoRegister()
{
	return UMultiplayerSessionsSubsystem::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UMultiplayerSessionsSubsystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *\n */" },
#endif
		{ "IncludePath", "MultiplayerSessionsSubsystem.h" },
		{ "ModuleRelativePath", "Public/MultiplayerSessionsSubsystem.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMultiplayerSessionsSubsystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UMultiplayerSessionsSubsystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGameInstanceSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSessionOW,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMultiplayerSessionsSubsystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UMultiplayerSessionsSubsystem_Statics::ClassParams = {
	&UMultiplayerSessionsSubsystem::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMultiplayerSessionsSubsystem_Statics::Class_MetaDataParams), Z_Construct_UClass_UMultiplayerSessionsSubsystem_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UMultiplayerSessionsSubsystem()
{
	if (!Z_Registration_Info_UClass_UMultiplayerSessionsSubsystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMultiplayerSessionsSubsystem.OuterSingleton, Z_Construct_UClass_UMultiplayerSessionsSubsystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UMultiplayerSessionsSubsystem.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UMultiplayerSessionsSubsystem);
UMultiplayerSessionsSubsystem::~UMultiplayerSessionsSubsystem() {}
// ********** End Class UMultiplayerSessionsSubsystem **********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h__Script_MultiplayerSessionOW_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UMultiplayerSessionsSubsystem, UMultiplayerSessionsSubsystem::StaticClass, TEXT("UMultiplayerSessionsSubsystem"), &Z_Registration_Info_UClass_UMultiplayerSessionsSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMultiplayerSessionsSubsystem), 1371392108U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h__Script_MultiplayerSessionOW_198979381(TEXT("/Script/MultiplayerSessionOW"),
	Z_CompiledInDeferFile_FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h__Script_MultiplayerSessionOW_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_MultiplayerSessionsSubsystem_h__Script_MultiplayerSessionOW_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
