// Fill out your copyright notice in the Description page of Project Settings.

#include "DownloadProgressPage.h"
#include "DownloadProgressBar.h"



UDownloadProgressPage::UDownloadProgressPage()
{

}

bool UDownloadProgressPage::OnStart()
{
	//if (UDownloadProgressBar* DownloadProgressBar = Cast<UDownloadProgressBar>(GetWidgetFromName("DownloadProgressBar")))
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

void UDownloadProgressPage::OnInit()
{

}

void UDownloadProgressPage::OnRelease()
{

}

void UDownloadProgressPage::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UDownloadProgressPage::OnTick(float InDeltaTimeSecond)
{

}

void UDownloadProgressPage::StartDownload()
{
	//m_ProgressBar->StartDownload();
}

void UDownloadProgressPage::UpdateProgress(float _Progress)
{
	/*if (!m_ProgressBar)
		return;
	m_ProgressBar->SetPercentProgress(_Progress / 100.0f);*/

	int32 mProgess = (int32)_Progress;
	FString mString = FString::FromInt(mProgess) + TEXT("%");

	m_progresstip->SetText(FText::FromString(mString));
}

void UDownloadProgressPage::UpdateShowText(FString _ShowText)
{
	m_tip->SetText(FText::FromString(_ShowText));
}
