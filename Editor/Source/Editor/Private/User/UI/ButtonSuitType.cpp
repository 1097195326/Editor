// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonSuitType.h"
#include "UIManager.h"
#include "RecommendStyleUMG.h"


UButtonSuitType::UButtonSuitType(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
}

bool UButtonSuitType::OnStart()
{
	
	
	
	return true;
	
}

void UButtonSuitType::OnInit()
{
	if (UCanvasPanel* CanvasPanelSuit = Cast<UCanvasPanel>(GetWidgetFromName("CanvasPanelSuit")))
	{
		m_CanvasPanelSuit = CanvasPanelSuit;
	}
	if (UTextBlock* TextBlockSuit = Cast<UTextBlock>(GetWidgetFromName("TextBlockSuit")))
	{
		m_TextBlockSuit = TextBlockSuit;
	}
	if (UButton* ButtonSuit = Cast<UButton>(GetWidgetFromName("ButtonSuit")))
	{
		m_ButtonSuit = ButtonSuit;
		UUIManager::GetInstance()->RegisterButtonClick(1, m_ButtonSuit, this, &UButtonSuitType::ClientButtonEvent);
	}
	if (UImage* ImageSuit = Cast<UImage>(GetWidgetFromName("ImageSuit")))
	{
		m_ImageSuit = ImageSuit;
	}
}

void UButtonSuitType::OnRelease()
{

}

void UButtonSuitType::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UButtonSuitType::ClientButtonEvent(int32 _pid)
{
	//BtnClickEvent.Broadcast(m_CategoryID);
}

void UButtonSuitType::OnTick(float InDeltaTimeSecond)
{

}

void UButtonSuitType::SetNormal()
{
	m_ImageSuit->SetOpacity(0.0f);
	m_TextBlockSuit->SetOpacity(0.5f);
	m_IsSelect = false;
	m_ButtonSuit->SetIsEnabled(true);
}

void UButtonSuitType::SetData(FString _name, int32 _categoryID)
{
	
	m_CategoryID = _categoryID;
	m_TextBlockSuit->SetText(FText::FromString(_name));
	m_Name = _name;
	SetNormal();
}

void UButtonSuitType::SetHover()
{
	m_ImageSuit->SetOpacity(1.0f);
	m_TextBlockSuit->SetOpacity(1.0f);
}

void UButtonSuitType::SetSelect()
{
	SetHover();
	m_IsSelect = true;
	m_ButtonSuit->SetIsEnabled(false);
}
