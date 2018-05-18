// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_WebPage.h"
#include "UIManager.h"

UWin_WebPage::UWin_WebPage(const FObjectInitializer& ObjectInitializer)
{
	m_BtnClose = nullptr;
	m_WebBuy = nullptr;
}

bool UWin_WebPage::OnStart()
{
	if(nullptr != m_BtnClose)
		UUIManager::GetInstance()->RegisterButtonClick(1, m_BtnClose, this, &UWin_WebPage::OnCloseWeb);

	return true;
}

void UWin_WebPage::OnInit()
{
	if (UButton * mTemp = Cast<UButton>(GetWidgetFromName("Button_Close")))
	{
		m_BtnClose = mTemp;
	}
	if (UWebBrowser * mTemp = Cast<UWebBrowser>(GetWidgetFromName("WebBrowser_Url")))
	{
		m_WebBuy = mTemp;
	}
	//	ЖўЮЌТы.
	if (UImage * ImgWeb = Cast<UImage>(GetWidgetFromName("Image_Mask")))
	{
		m_ImageWeb = ImgWeb;
	}
}

void UWin_WebPage::OnRelease()
{
	m_BtnClose = nullptr;

	m_WebBuy = nullptr;

	if(nullptr != m_BtnClose)
		UUIManager::GetInstance()->RemoveButtonClick(m_BtnClose);

	if (nullptr != m_ImageWeb && nullptr != m_ImageWeb->Brush.GetResourceObject())
	{
		UTexture2D * mTexture = (UTexture2D *)m_ImageWeb->Brush.GetResourceObject();

		mTexture->ReleaseResource();
	}
}

void UWin_WebPage::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_WebPage::OnTick(float InDeltaTimeSecond)
{

}

void UWin_WebPage::OnBackstageUI()
{

}

void UWin_WebPage::OnCloseWeb(int32 _pid)
{
	this->RemoveFromViewport();
}
