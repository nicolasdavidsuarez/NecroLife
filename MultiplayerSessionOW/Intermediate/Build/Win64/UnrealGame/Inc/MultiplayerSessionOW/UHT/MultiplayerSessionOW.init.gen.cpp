// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMultiplayerSessionOW_init() {}
	MULTIPLAYERSESSIONOW_API UFunction* Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature();
	MULTIPLAYERSESSIONOW_API UFunction* Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature();
	MULTIPLAYERSESSIONOW_API UFunction* Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_MultiplayerSessionOW;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_MultiplayerSessionOW()
	{
		if (!Z_Registration_Info_UPackage__Script_MultiplayerSessionOW.OuterSingleton)
		{
			static UObject* (*const SingletonFuncArray[])() = {
				(UObject* (*)())Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlCrearSesionCompletada__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlDestruirSesionCompletada__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_MultiplayerSessionOW_MultijugadorAlIniciarSesionCompletada__DelegateSignature,
			};
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/MultiplayerSessionOW",
				SingletonFuncArray,
				UE_ARRAY_COUNT(SingletonFuncArray),
				PKG_CompiledIn | 0x00000000,
				0x4462573C,
				0x66449D09,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_MultiplayerSessionOW.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_MultiplayerSessionOW.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_MultiplayerSessionOW(Z_Construct_UPackage__Script_MultiplayerSessionOW, TEXT("/Script/MultiplayerSessionOW"), Z_Registration_Info_UPackage__Script_MultiplayerSessionOW, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x4462573C, 0x66449D09));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
