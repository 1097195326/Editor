// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_DisplaySetting.h"
#include "GModuleManager.h"
#include "GameDirector.h"
#include "GLanguageTool.h"
#include "UIManager.h"
#include "EditorFunctionalTool.h"

UWin_DisplaySetting::UWin_DisplaySetting(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	//	初始化变量
	m_BtnQualityLow = nullptr;
	m_BtnQualityHigh = nullptr;
	m_BtnQualityHigher = nullptr;

	m_TextQualityLevel = nullptr;
	m_TextQualityLow = nullptr;
	m_TextQualityHigh = nullptr;
	m_TextQualityHigher = nullptr;
}

bool UWin_DisplaySetting::OnStart()
{
// 	m_LoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");
// 
// 	EQualityLevel mQualityLevel = GetCurrentQuality();
// 
// 	if (EQualityLevel::E_Low == mQualityLevel)
// 		SelectQualityLevel(1);
// 
// 	else if (EQualityLevel::E_High == mQualityLevel)
// 		SelectQualityLevel(2);
// 
// 	else if (EQualityLevel::E_Highest == mQualityLevel)
// 		SelectQualityLevel(3);

	return true;
}

void UWin_DisplaySetting::OnInit()
{
// 	//	获取标题汉字.
// 	if (UTextBlock* TextQualityLevel = Cast<UTextBlock>(GetWidgetFromName("Text_QualityLevel")))
// 	{
// 		m_TextQualityLevel = TextQualityLevel;
// 
// 		// 获取文字.
// 		FString mQualityLevel = GLanguageTool::GetInstance()->GetLanguage(TEXT("key107"));
// 		m_TextQualityLevel->SetText(FText::FromString(mQualityLevel));
// 	}
// 
// 	//	获取画质低.
// 	if (UButton* BtnQualityLow = Cast<UButton>(GetWidgetFromName("Button_QualityLow")))
// 	{
// 		m_BtnQualityLow = BtnQualityLow;
// 
// 		UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnQualityLow, this, &UWin_DisplaySetting::SelectQualityLevel);
// 	}
// 	if (UTextBlock* TextQualityLow = Cast<UTextBlock>(GetWidgetFromName("Text_QualityLow")))
// 	{
// 		m_TextQualityLow = TextQualityLow;
// 
// 		// 获取文字.
// 		FString mQualityLow = GLanguageTool::GetInstance()->GetLanguage(TEXT("key115"));
// 		m_TextQualityLow->SetText(FText::FromString(mQualityLow));
// 	}
// 	if (UImage* ImageQualityLow = Cast<UImage>(GetWidgetFromName("Image_QualityLow")))
// 	{
// 		m_ImageQualityLow = ImageQualityLow;
// 	}
// 
// 	//	画面质量中.
// 	if (UButton* BtnQualityHigh = Cast<UButton>(GetWidgetFromName("Button_QualityHigh")))
// 	{
// 		m_BtnQualityHigh = BtnQualityHigh;
// 
// 		UUIManager::GetInstance()->RegisterButtonClick(2, m_BtnQualityHigh, this, &UWin_DisplaySetting::SelectQualityLevel);
// 	}
// 	if (UTextBlock* TextQualityHigh = Cast<UTextBlock>(GetWidgetFromName("Text_QualityHigh")))
// 	{
// 		m_TextQualityHigh = TextQualityHigh;
// 
// 		// 获取文字.
// 		FString mQualityHigh = GLanguageTool::GetInstance()->GetLanguage(TEXT("key116"));
// 		m_TextQualityHigh->SetText(FText::FromString(mQualityHigh));
// 	}
// 	if (UImage* ImageQualityHigh = Cast<UImage>(GetWidgetFromName("Image_QualityHigh")))
// 	{
// 		m_ImageQualityHigh = ImageQualityHigh;
// 	}
// 
// 	//	画面质量高.
// 	if (UButton* BtnQualityHigher = Cast<UButton>(GetWidgetFromName("Button_QualityHigher")))
// 	{
// 		m_BtnQualityHigher = BtnQualityHigher;
// 
// 		UUIManager::GetInstance()->RegisterButtonClick(3, m_BtnQualityHigher, this, &UWin_DisplaySetting::SelectQualityLevel);
// 	}
// 	if (UTextBlock* TextQualityHigher = Cast<UTextBlock>(GetWidgetFromName("Text_QualityHigher")))
// 	{
// 		m_TextQualityHigher = TextQualityHigher;
// 
// 		// 获取文字.
// 		FString mQualityHigher = GLanguageTool::GetInstance()->GetLanguage(TEXT("key117"));
// 		m_TextQualityHigher->SetText(FText::FromString(mQualityHigher));
// 	}
// 	if (UImage* ImageQualityHigher = Cast<UImage>(GetWidgetFromName("Image_QualityHigher")))
// 	{
// 		m_ImageQualityHigher = ImageQualityHigher;
// 	}
}

void UWin_DisplaySetting::OnRelease()
{
// 	UUIManager::GetInstance()->RemoveButtonClick(m_BtnQualityLow);
// 
// 	UUIManager::GetInstance()->RemoveButtonClick(m_BtnQualityHigh);
// 
// 	UUIManager::GetInstance()->RemoveButtonClick(m_BtnQualityHigher);
}

void UWin_DisplaySetting::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_DisplaySetting::OnTick(float InDeltaTimeSecond)
{

}

void UWin_DisplaySetting::SelectQualityLevel(int32 _pid)
{
// 	//	画面质量低
// 	if (_pid == 1)
// 	{
// 		SelectClick(EQualityLevel::E_Low);
// 
// 		m_BtnQualityLow->SetIsEnabled(false);
// 		m_BtnQualityHigh->SetIsEnabled(true);
// 		m_BtnQualityHigher->SetIsEnabled(true);
// 
// 		m_ImageQualityLow->SetVisibility(ESlateVisibility::Visible);
// 		m_ImageQualityHigh->SetVisibility(ESlateVisibility::Hidden);
// 		m_ImageQualityHigher->SetVisibility(ESlateVisibility::Hidden);
// 	}
// 
// 	//	画面质量中
// 	if (_pid == 2)
// 	{
// 		SelectClick(EQualityLevel::E_High);
// 
// 		m_BtnQualityLow->SetIsEnabled(true);
// 		m_BtnQualityHigh->SetIsEnabled(false);
// 		m_BtnQualityHigher->SetIsEnabled(true);
// 
// 		m_ImageQualityLow->SetVisibility(ESlateVisibility::Hidden);
// 		m_ImageQualityHigh->SetVisibility(ESlateVisibility::Visible);
// 		m_ImageQualityHigher->SetVisibility(ESlateVisibility::Hidden);
// 	}
// 
// 	//	画面质量高
// 	if (_pid == 3)
// 	{
// 		SelectClick(EQualityLevel::E_Highest);
// 
// 		m_BtnQualityLow->SetIsEnabled(true);
// 		m_BtnQualityHigh->SetIsEnabled(true);
// 		m_BtnQualityHigher->SetIsEnabled(false);
// 
// 		m_ImageQualityLow->SetVisibility(ESlateVisibility::Hidden);
// 		m_ImageQualityHigh->SetVisibility(ESlateVisibility::Hidden);
// 		m_ImageQualityHigher->SetVisibility(ESlateVisibility::Visible);
// 	}
// 	
// 	this->RemoveFromViewport();
}

void UWin_DisplaySetting::SelectClick(EQualityLevel _level)
{
	//EditorFunctionalTool::SetQualityByShortcut(_level);
	
	//m_LoginModule->GetUserData()->SaveSettingDisplayLevel(_level);
}

EQualityLevel UWin_DisplaySetting::GetCurrentQuality()
{
	return EQualityLevel::E_High;
	//return m_LoginModule->GetUserData()->GetSettingDisplayLevel();
}

