// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "WidgetSessionSerch.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeWidgetSessionSerch() {}

// ********** Begin Cross Module References ********************************************************
MULTIPLAYERSESSIONOW_API UClass* Z_Construct_UClass_UWidgetSessionSerch();
MULTIPLAYERSESSIONOW_API UClass* Z_Construct_UClass_UWidgetSessionSerch_NoRegister();
UMG_API UClass* Z_Construct_UClass_UButton_NoRegister();
UMG_API UClass* Z_Construct_UClass_UTextBlock_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_MultiplayerSessionOW();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UWidgetSessionSerch Function OnButtonClicked *****************************
struct Z_Construct_UFunction_UWidgetSessionSerch_OnButtonClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/WidgetSessionSerch.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWidgetSessionSerch_OnButtonClicked_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWidgetSessionSerch, nullptr, "OnButtonClicked", nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWidgetSessionSerch_OnButtonClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWidgetSessionSerch_OnButtonClicked_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UWidgetSessionSerch_OnButtonClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWidgetSessionSerch_OnButtonClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWidgetSessionSerch::execOnButtonClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnButtonClicked();
	P_NATIVE_END;
}
// ********** End Class UWidgetSessionSerch Function OnButtonClicked *******************************

// ********** Begin Class UWidgetSessionSerch Function SetData *************************************
struct Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics
{
	struct WidgetSessionSerch_eventSetData_Parms
	{
		FText UserN;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/WidgetSessionSerch.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FTextPropertyParams NewProp_UserN;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics::NewProp_UserN = { "UserN", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(WidgetSessionSerch_eventSetData_Parms, UserN), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics::NewProp_UserN,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UWidgetSessionSerch, nullptr, "SetData", Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics::PropPointers), sizeof(Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics::WidgetSessionSerch_eventSetData_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics::Function_MetaDataParams), Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics::WidgetSessionSerch_eventSetData_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UWidgetSessionSerch_SetData()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UWidgetSessionSerch_SetData_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UWidgetSessionSerch::execSetData)
{
	P_GET_PROPERTY(FTextProperty,Z_Param_UserN);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetData(Z_Param_UserN);
	P_NATIVE_END;
}
// ********** End Class UWidgetSessionSerch Function SetData ***************************************

// ********** Begin Class UWidgetSessionSerch ******************************************************
void UWidgetSessionSerch::StaticRegisterNativesUWidgetSessionSerch()
{
	UClass* Class = UWidgetSessionSerch::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "OnButtonClicked", &UWidgetSessionSerch::execOnButtonClicked },
		{ "SetData", &UWidgetSessionSerch::execSetData },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
FClassRegistrationInfo Z_Registration_Info_UClass_UWidgetSessionSerch;
UClass* UWidgetSessionSerch::GetPrivateStaticClass()
{
	using TClass = UWidgetSessionSerch;
	if (!Z_Registration_Info_UClass_UWidgetSessionSerch.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("WidgetSessionSerch"),
			Z_Registration_Info_UClass_UWidgetSessionSerch.InnerSingleton,
			StaticRegisterNativesUWidgetSessionSerch,
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
	return Z_Registration_Info_UClass_UWidgetSessionSerch.InnerSingleton;
}
UClass* Z_Construct_UClass_UWidgetSessionSerch_NoRegister()
{
	return UWidgetSessionSerch::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UWidgetSessionSerch_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "WidgetSessionSerch.h" },
		{ "ModuleRelativePath", "Public/WidgetSessionSerch.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ConectSessionButton_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/WidgetSessionSerch.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_UserName_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/WidgetSessionSerch.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ConectSessionButton;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_UserName;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UWidgetSessionSerch_OnButtonClicked, "OnButtonClicked" }, // 180243950
		{ &Z_Construct_UFunction_UWidgetSessionSerch_SetData, "SetData" }, // 3449197888
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWidgetSessionSerch>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWidgetSessionSerch_Statics::NewProp_ConectSessionButton = { "ConectSessionButton", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWidgetSessionSerch, ConectSessionButton), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ConectSessionButton_MetaData), NewProp_ConectSessionButton_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWidgetSessionSerch_Statics::NewProp_UserName = { "UserName", nullptr, (EPropertyFlags)0x0010000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UWidgetSessionSerch, UserName), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_UserName_MetaData), NewProp_UserName_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWidgetSessionSerch_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWidgetSessionSerch_Statics::NewProp_ConectSessionButton,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWidgetSessionSerch_Statics::NewProp_UserName,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWidgetSessionSerch_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UWidgetSessionSerch_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_MultiplayerSessionOW,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UWidgetSessionSerch_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UWidgetSessionSerch_Statics::ClassParams = {
	&UWidgetSessionSerch::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UWidgetSessionSerch_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UWidgetSessionSerch_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UWidgetSessionSerch_Statics::Class_MetaDataParams), Z_Construct_UClass_UWidgetSessionSerch_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UWidgetSessionSerch()
{
	if (!Z_Registration_Info_UClass_UWidgetSessionSerch.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWidgetSessionSerch.OuterSingleton, Z_Construct_UClass_UWidgetSessionSerch_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UWidgetSessionSerch.OuterSingleton;
}
UWidgetSessionSerch::UWidgetSessionSerch(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UWidgetSessionSerch);
UWidgetSessionSerch::~UWidgetSessionSerch() {}
// ********** End Class UWidgetSessionSerch ********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_WidgetSessionSerch_h__Script_MultiplayerSessionOW_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UWidgetSessionSerch, UWidgetSessionSerch::StaticClass, TEXT("UWidgetSessionSerch"), &Z_Registration_Info_UClass_UWidgetSessionSerch, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWidgetSessionSerch), 1514813199U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_WidgetSessionSerch_h__Script_MultiplayerSessionOW_3172883653(TEXT("/Script/MultiplayerSessionOW"),
	Z_CompiledInDeferFile_FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_WidgetSessionSerch_h__Script_MultiplayerSessionOW_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MenuRPGOW_Plugins_MultiplayerSessionOW_Source_MultiplayerSessionOW_Public_WidgetSessionSerch_h__Script_MultiplayerSessionOW_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
