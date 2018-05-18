// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_LanguageSetting.h"
#include "GModuleManager.h"
#include "GameDirector.h"
#include "GLanguageTool.h"
#include "UIManager.h"

UWin_LanguageSetting::UWin_LanguageSetting(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	m_BtnChinese = nullptr;
	m_BtnEnglish = nullptr;

	m_TextChinese = nullptr;
	m_TextEnglish = nullptr;
	m_TextLanguage = nullptr;
}

bool UWin_LanguageSetting::OnStart()
{
// 	m_LoginModule = GModuleManager::GetInstance()->GetModule<LoginModule>("LoginModule");
// 
// 	//	获取标题汉字.
// 	if (UTextBlock* TextLanguage = Cast<UTextBlock>(GetWidgetFromName("Text_Language")))
// 	{
// 		m_TextLanguage = TextLanguage;
// 
// 		// 获取文字.
// 		FString mLanguage = GLanguageTool::GetInstance()->GetLanguage(TEXT("key108"));
// 		m_TextLanguage->SetText(FText::FromString(mLanguage));
// 	}
// 
// 	//	获取中文设置按钮.
// 	if (UButton* BtnChinese = Cast<UButton>(GetWidgetFromName("Button_Chinese")))
// 	{
// 		m_BtnChinese = BtnChinese;
// 
// 		UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnChinese, this, &UWin_LanguageSetting::SelectLanguage);
// 	}
// 	if (UTextBlock* TextChinese = Cast<UTextBlock>(GetWidgetFromName("Text_Chinese")))
// 	{
// 		m_TextChinese = TextChinese;
// 
// 		// 获取文字.
// 		FString mChinese = GLanguageTool::GetInstance()->GetLanguage(TEXT("key118"));
// 		m_TextChinese->SetText(FText::FromString(mChinese));
// 	}
// 
// 	//	获取英文设置按钮.
// 	if (UButton* BtnEnglish = Cast<UButton>(GetWidgetFromName("Button_English")))
// 	{
// 		m_BtnEnglish = BtnEnglish;
// 
// 		UUIManager::GetInstance()->RegisterButtonClick(2, m_BtnEnglish, this, &UWin_LanguageSetting::SelectLanguage);
// 	}
// 	if (UTextBlock* TextEnglish = Cast<UTextBlock>(GetWidgetFromName("Text_English")))
// 	{
// 		m_TextEnglish = TextEnglish;
// 
// 		// 获取文字.
// 		FString mEnglish = GLanguageTool::GetInstance()->GetLanguage(TEXT("key119"));
// 		m_TextEnglish->SetText(FText::FromString(mEnglish));
// 	}

	return true;
}

void UWin_LanguageSetting::OnInit()
{

}

void UWin_LanguageSetting::OnRelease()
{
// 	UUIManager::GetInstance()->RemoveButtonClick(m_BtnChinese);
// 
// 	UUIManager::GetInstance()->RemoveButtonClick(m_BtnEnglish);
}

void UWin_LanguageSetting::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_LanguageSetting::OnTick(float InDeltaTimeSecond)
{

}

void UWin_LanguageSetting::SelectLanguage(int32 _pid)
{
// 	if (_pid == 1)
// 	{
// 
// 	}
// 
// 	else if (_pid == 2)
// 	{
// 
// 	}
// 	this->RemoveFromViewport();
}
