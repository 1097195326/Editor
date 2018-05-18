// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_ButtonSuitType.h"
#include "UIManager.h"
#include "Win_RecommendStyleUMG.h"


UWin_ButtonSuitType::UWin_ButtonSuitType(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
}

bool UWin_ButtonSuitType::OnStart()
{
	
	
	
	return true;
	
}

void UWin_ButtonSuitType::OnInit()
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
		UUIManager::GetInstance()->RegisterButtonClick(1, m_ButtonSuit, this, &UWin_ButtonSuitType::ClientButtonEvent);
	}
	if (UImage* ImageSuit = Cast<UImage>(GetWidgetFromName("ImageSuit")))
	{
		m_ImageSuit = ImageSuit;
	}
}

void UWin_ButtonSuitType::OnRelease()
{

}

void UWin_ButtonSuitType::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_ButtonSuitType::ClientButtonEvent(int32 _pid)
{
	//BtnClickEvent.Broadcast(m_CategoryID);
}

void UWin_ButtonSuitType::OnTick(float InDeltaTimeSecond)
{

}

void UWin_ButtonSuitType::SetNormal()
{
	m_ImageSuit->SetOpacity(0.0f);
	m_TextBlockSuit->SetOpacity(0.5f);
	m_IsSelect = false;
	m_ButtonSuit->SetIsEnabled(true);
}

void UWin_ButtonSuitType::SetData(FString _name, int32 _categoryID)
{
	
	m_CategoryID = _categoryID;
	m_TextBlockSuit->SetText(FText::FromString(_name));
	m_Name = _name;
	SetNormal();
}

void UWin_ButtonSuitType::SetHover()
{
	m_ImageSuit->SetOpacity(1.0f);
	m_TextBlockSuit->SetOpacity(1.0f);
}

void UWin_ButtonSuitType::SetSelect()
{
	SetHover();
	m_IsSelect = true;
	m_ButtonSuit->SetIsEnabled(false);
}
