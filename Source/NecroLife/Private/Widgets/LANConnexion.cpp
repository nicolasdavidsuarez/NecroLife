// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/LANConnexion.h"

#include "Kismet/GameplayStatics.h"

void ULANConnexion::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	FInputModeUIOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(true);
	Button_Create->OnClicked.AddDynamic(this,&ThisClass::OnButtonCreateClicked);
	Button_Join->OnClicked.AddDynamic(this,&ThisClass::OnButtonJoinClicked);
}

void ULANConnexion::OnButtonCreateClicked()
{
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);
	UGameplayStatics::OpenLevelBySoftObjectPtr(this,World,true,TEXT("listen"));
}

void ULANConnexion::OnButtonJoinClicked()
{
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->SetShowMouseCursor(false);
	const FString IpStr = EditableTextBox_Ip->GetText().ToString();
	UGameplayStatics::OpenLevel(this,*IpStr);
}
