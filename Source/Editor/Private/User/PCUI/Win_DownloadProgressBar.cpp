// Fill out your copyright notice in the Description page of Project Settings.

#include "Win_DownloadProgressBar.h"




UWin_DownloadProgressBar::UWin_DownloadProgressBar() :
	showStr(TEXT("")),
	MAXDOWNLOADFILE(0)
{

}

void UWin_DownloadProgressBar::SetDefaultStr(const FString & str)
{
	DefaultStr = str;
}

void UWin_DownloadProgressBar::CallbackNotifyJsonProgress()
{
	AddStep();
	ChangeProgress();
	UpdateShowText();
}

void UWin_DownloadProgressBar::CallbackSystemJsonsFinished()
{
	AddPart();
	currentStep = 0;
	StartDownloadTwo();
}

void UWin_DownloadProgressBar::CallbackDownloadOtherProgress(FString inFileName, float inPercent)
{
	if (inPercent > 0.999f && inPercent < 1.001f) {
		AddStep();
		ChangeProgress();
	}
}

void UWin_DownloadProgressBar::LocalAssetCheckFinished()
{
	// 本地资源校验完成.
	//UE_LOG(ClothEditor, Log, TEXT("LocalAssetCheckFinished Check Finished"));
// 	UEditorGameInstance* ThisGameInstance = Cast<UEditorGameInstance>(GetWorld()->GetGameInstance());
// 	ThisGameInstance->GetResourceManagerRepository()->AfterLoadResource();
	DownloadFinished();
}

void UWin_DownloadProgressBar::SetProgress(float progress)
{
	realProgress = progress;
}

float UWin_DownloadProgressBar::GetProgress()
{
	return realProgress;
}

void UWin_DownloadProgressBar::UpdateShowProgress(float progress)
{
	if (progress > 100.0f)progress = 100.0f;
	SetProgress(progress);
	onUpdateProgress.Broadcast(progress);
}

void UWin_DownloadProgressBar::UpdateShowText()
{
	SetShowText(DefaultStr + TEXT("[") + FString::FromInt(currentPart + 1) + TEXT("/") + FString::FromInt(STEPOTHER + 1) + TEXT("]"));
	onUpdateShowText.Broadcast(showStr);
}

void UWin_DownloadProgressBar::SetShowText(const FString & str)
{
	showStr = str;
}

void UWin_DownloadProgressBar::ChangeProgress()
{
	if (currentPart == STEPJSON)
	{
		UpdateShowProgress(((float)currentStep) / MAXJSONSTEP*100.f);
	}
	else if (currentPart == STEPOTHER) {
		if (MAXDOWNLOADFILE > 0) {
			UpdateShowProgress(((float)currentStep) / MAXDOWNLOADFILE*100.f);
		}
	}
	else {}
}

void UWin_DownloadProgressBar::StartDownload()
{
 	//Reset();
  //	UEditorGameInstance* ThisGameInstance = Cast<UEditorGameInstance>(GetWorld()->GetGameInstance());
  //	check(ThisGameInstance);
  //
  //	FResourceManagerRepository *ResourceManagerRepository = ThisGameInstance->GetResourceManagerRepository();
  //	//请求系统Json.
  //	ResourceManagerRepository->RequestSystemJsons(FOnRequestSystemJsonFinished::CreateUObject(this, &UDownloadProgressView::CallbackSystemJsonsFinished),
  //		FOnRequestJsonProgressIncrement::CreateUObject(this, &UDownloadProgressView::CallbackNotifyJsonProgress));

}

void UWin_DownloadProgressBar::StartDownloadTwo()
{
// 	UpdateShowText();
// 	UEditorGameInstance* ThisGameInstance = Cast<UEditorGameInstance>(GetWorld()->GetGameInstance());
// 	ThisGameInstance->GetDownloadProgressEvent().AddDynamic(this, &UDownloadProgressView::CallbackDownloadOtherProgress);
// 	MAXDOWNLOADFILE = ThisGameInstance->GetResourceManagerRepository()->CheckLocalAsset(FOnOSSRequestFinished::CreateUObject(this, &UDownloadProgressView::LocalAssetCheckFinished));
// 	if (MAXDOWNLOADFILE < 1)
// 	{
// 		LocalAssetCheckFinished();
// 	}
}

void UWin_DownloadProgressBar::DownloadFinished()
{
	onDownloadFinished.Broadcast();

}

void UWin_DownloadProgressBar::AddStep()
{
	currentStep++;
}

void UWin_DownloadProgressBar::AddPart()
{
	if (currentPart == STEPJSON)
	{
		currentPart = STEPOTHER;
	}
}

void UWin_DownloadProgressBar::Reset()
{
	currentStep = 0;
	currentPart = STEPJSON;
	SetShowText(TEXT(""));
}