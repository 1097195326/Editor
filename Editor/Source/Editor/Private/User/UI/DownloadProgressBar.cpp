// Fill out your copyright notice in the Description page of Project Settings.

#include "DownloadProgressBar.h"




UDownloadProgressBar::UDownloadProgressBar() :
	showStr(TEXT("")),
	MAXDOWNLOADFILE(0)
{

}

void UDownloadProgressBar::SetDefaultStr(const FString & str)
{
	DefaultStr = str;
}

void UDownloadProgressBar::CallbackNotifyJsonProgress()
{
	AddStep();
	ChangeProgress();
	UpdateShowText();
}

void UDownloadProgressBar::CallbackSystemJsonsFinished()
{
	AddPart();
	currentStep = 0;
	StartDownloadTwo();
}

void UDownloadProgressBar::CallbackDownloadOtherProgress(FString inFileName, float inPercent)
{
	if (inPercent > 0.999f && inPercent < 1.001f) {
		AddStep();
		ChangeProgress();
	}
}

void UDownloadProgressBar::LocalAssetCheckFinished()
{
	// 本地资源校验完成.
	//UE_LOG(ClothEditor, Log, TEXT("LocalAssetCheckFinished Check Finished"));
// 	UEditorGameInstance* ThisGameInstance = Cast<UEditorGameInstance>(GetWorld()->GetGameInstance());
// 	ThisGameInstance->GetResourceManagerRepository()->AfterLoadResource();
	DownloadFinished();
}

void UDownloadProgressBar::SetProgress(float progress)
{
	realProgress = progress;
}

float UDownloadProgressBar::GetProgress()
{
	return realProgress;
}

void UDownloadProgressBar::UpdateShowProgress(float progress)
{
	if (progress > 100.0f)progress = 100.0f;
	SetProgress(progress);
	onUpdateProgress.Broadcast(progress);
}

void UDownloadProgressBar::UpdateShowText()
{
	SetShowText(DefaultStr + TEXT("[") + FString::FromInt(currentPart + 1) + TEXT("/") + FString::FromInt(STEPOTHER + 1) + TEXT("]"));
	onUpdateShowText.Broadcast(showStr);
}

void UDownloadProgressBar::SetShowText(const FString & str)
{
	showStr = str;
}

void UDownloadProgressBar::ChangeProgress()
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

void UDownloadProgressBar::StartDownload()
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

void UDownloadProgressBar::StartDownloadTwo()
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

void UDownloadProgressBar::DownloadFinished()
{
	onDownloadFinished.Broadcast();

}

void UDownloadProgressBar::AddStep()
{
	currentStep++;
}

void UDownloadProgressBar::AddPart()
{
	if (currentPart == STEPJSON)
	{
		currentPart = STEPOTHER;
	}
}

void UDownloadProgressBar::Reset()
{
	currentStep = 0;
	currentPart = STEPJSON;
	SetShowText(TEXT(""));
}