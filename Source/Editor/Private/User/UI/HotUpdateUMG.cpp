#include "HotUpdateUMG.h"
#include "GModuleManager.h"
#include "MsgCenter.h"
#include "Editor.h"
#include "UIManager.h"
#include "GLanguageTool.h"
#include "UpdateUIController.h"
#include <iomanip>


bool UHotUpdateUMG::OnStart()
{
	FString mTip = GLanguageTool::GetInstance()->GetLanguage(TEXT("key104"));
	FString mText_This = GLanguageTool::GetInstance()->GetLanguage(TEXT("key105"));
	FString mText_Speed = GLanguageTool::GetInstance()->GetLanguage(TEXT("key106"));
	m_Text_Tip->SetText(FText::FromString(mTip));
	m_Text_This->SetText(FText::FromString(mText_This));
	m_Text_Speed->SetText(FText::FromString(mText_Speed));
	return true;
}

void UHotUpdateUMG::OnInit()
{
	//初始化UI.
	if (UTextBlock *mTextBlock = Cast<UTextBlock>(GetWidgetFromName("Text_Tip")))
	{
		m_Text_Tip = mTextBlock;
	}
	if (UTextBlock *mTextBlock = Cast<UTextBlock>(GetWidgetFromName("Text_This")))
	{
		m_Text_This = mTextBlock;
	}
	if (UTextBlock *mTextBlock = Cast<UTextBlock>(GetWidgetFromName("Text_Var_Size")))
	{
		m_Text_Var_Size = mTextBlock;
	}
	if (UTextBlock *mTextBlock = Cast<UTextBlock>(GetWidgetFromName("Text_Speed")))
	{
		m_Text_Speed = mTextBlock;
	}
	if (UTextBlock *mTextBlock = Cast<UTextBlock>(GetWidgetFromName("Text_Var_Speed")))
	{
		m_Text_Var_Speed = mTextBlock;
	}
	if (UTextBlock *mTextBlock = Cast<UTextBlock>(GetWidgetFromName("Text_Progress")))
	{
		m_Text_Progress = mTextBlock;
	}
	if (UProgressBar *mBarProgress = Cast<UProgressBar>(GetWidgetFromName("Bar_Progress")))
	{
		m_Bar_Progress = mBarProgress;
	}
	m_Text_Var_Speed->SetVisibility(ESlateVisibility::Hidden);

	m_Text_Speed->SetVisibility(ESlateVisibility::Hidden);

	m_Text_This->SetVisibility(ESlateVisibility::Hidden);
}

void UHotUpdateUMG::OnRelease()
{

	m_Text_Tip = nullptr;
	m_Text_This = nullptr;
	m_Text_Var_Size = nullptr;
	m_Text_Speed = nullptr;
	m_Text_Var_Speed = nullptr;
	m_Text_Progress = nullptr;
	m_Bar_Progress = nullptr;
}

void UHotUpdateUMG::OnSetParams(int32 _pid, FString & _content, ...)
{

}

void UHotUpdateUMG::OnTick(float InDeltaTimeSecond)
{

}



void UHotUpdateUMG::OnActivationUI()
{

	this->SetVisibility(ESlateVisibility::Visible);
}

void UHotUpdateUMG::OnBackstageUI()
{

	this->SetVisibility(ESlateVisibility::Hidden);
}

void UHotUpdateUMG::Reset()
{
	m_Bar_Progress->SetPercent(0);
	m_Text_Progress->SetText(FText::FromString("0%"));
	m_Text_Var_Size->SetText(FText::FromString("0M"));
	m_Text_Var_Speed->SetText(FText::FromString("0Kb"));
}

void UHotUpdateUMG::SetProgress(DownlaodInfoStruct *_DownloadInfoStruct)
{
	m_Text_Tip->SetText(FText::FromString(_DownloadInfoStruct->m_DownloadMsg));

	m_Text_This->SetVisibility(ESlateVisibility::Visible);
	//下载进度.
	m_Bar_Progress->SetPercent(_DownloadInfoStruct->m_Progress);
	FString mProgress = FString::Printf(TEXT("%.1f"), _DownloadInfoStruct->m_Progress*100) + "%";
	m_Text_Progress->SetText(FText::FromString(mProgress));

	//下载文件大小.
	FString mSize = FString::Printf(TEXT("%.1f"), _DownloadInfoStruct->m_Size) + "M";
	m_Text_Var_Size->SetText(FText::FromString(mSize));

	//当前下载速度.
	if (_DownloadInfoStruct->m_Speed == 0)
	{
		m_Text_Var_Speed->SetVisibility(ESlateVisibility::Hidden);

		m_Text_Speed->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		m_Text_Var_Speed->SetVisibility(ESlateVisibility::Visible);

		m_Text_Speed->SetVisibility(ESlateVisibility::Visible);

		FString mSpeed = FString::Printf(TEXT("%.1f"), _DownloadInfoStruct->m_Speed) + "KB/s";

		m_Text_Var_Speed->SetText(FText::FromString(mSpeed));
	}
}
