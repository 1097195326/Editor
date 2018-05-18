// Fill out your copyright notice in the Description page of Project Settings.

#include "GCore.h"
#include "BaseUI.h"

UBaseUI::UBaseUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	 

}

void UBaseUI::OnTick(float InDeltaTime)
{

}

void UBaseUI::BackstageUI()
{
	SetVisibility(ESlateVisibility::Hidden);

	OnBackstageUI();
}

void UBaseUI::ActivationUI()
{
	SetVisibility(ESlateVisibility::Visible);

	OnActivationUI();
}

void UBaseUI::GLoad()
{
	OnLoad();
}

void UBaseUI::GRelease()
{
	OnRelease();
}

void UBaseUI::OnBackstageUI()
{

}

void UBaseUI::OnActivationUI()
{

}

bool UBaseUI::OnStart()
{
	return true;
}

void UBaseUI::OnInit()
{

}

void UBaseUI::NativeDestruct()
{
	Super::NativeDestruct();
	m_isDestroyed = true;
	GRelease();
}


void UBaseUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	OnTick(InDeltaTime);
}

void UBaseUI::OnRelease()
{

}

void UBaseUI::OnLoad()
{

}

void UBaseUI::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UBaseUI::NativeConstruct()
{
	Super::NativeConstruct();

	OnStart();
}

UBaseUI::~UBaseUI()
{
	
}

bool UBaseUI::Initialize()   
{
	if (!Super::Initialize())
	{
		return false;
	}

	OnInit();

	return true;
}



