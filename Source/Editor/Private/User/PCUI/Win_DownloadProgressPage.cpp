// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_DownloadProgressPage.h"
#include "Win_DownloadProgressBar.h"



UWin_DownloadProgressPage::UWin_DownloadProgressPage()
{

}

bool UWin_DownloadProgressPage::OnStart()
{
	//if (UWin_DownloadProgressBar* DownloadProgressBar = Cast<UWin_DownloadProgressBar>(GetWidgetFromName("DownloadProgressBar")))
	//{
	//	//m_ProgressBar = DownloadProgressBar;
	//}

	if (UTextBlock* progresstip = Cast<UTextBlock>(GetWidgetFromName("progresstip")))
	{
		m_progresstip = progresstip;
	}


	if (UTextBlock* tip = Cast<UTextBlock>(GetWidgetFromName("tip")))
	{
		m_tip = tip;
	}
	

// 	m_ProgressBar->onUpdateProgress.BindUObject(this, &UDownloadProgressPage::UpdateProgress);
// 	m_ProgressBar->onUpdateShowText.BindUObject(this, &UDownloadProgressPage::UpdateShowText);
	return true;
}

void UWin_DownloadProgressPage::OnInit()
{

}

void UWin_DownloadProgressPage::OnRelease()
{

}

void UWin_DownloadProgressPage::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UWin_DownloadProgressPage::OnTick(float InDeltaTimeSecond)
{

}

void UWin_DownloadProgressPage::StartDownload()
{
	//m_ProgressBar->StartDownload();
}

void UWin_DownloadProgressPage::UpdateProgress(float _Progress)
{
	/*if (!m_ProgressBar)
		return;
	m_ProgressBar->SetPercentProgress(_Progress / 100.0f);*/

	int32 mProgess = (int32)_Progress;
	FString mString = FString::FromInt(mProgess) + TEXT("%");

	m_progresstip->SetText(FText::FromString(mString));
}

void UWin_DownloadProgressPage::UpdateShowText(FString _ShowText)
{
	m_tip->SetText(FText::FromString(_ShowText));
}
