// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "DownloadProgressBar.generated.h"

#define MAXJSONSTEP 8
#define FULLPROGRESS 100
#define STEPJSON 0
#define STEPOTHER 1

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateProgress, float, progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateShowText, FString, str);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDownloadFinishedEvent);

/**
 * 
 */
UCLASS()
class EDITOR_API UDownloadProgressBar : public UBaseUI
{
	GENERATED_BODY()
	
public:
	UDownloadProgressBar();
protected:
	int32 currentStep;
	int32 currentPart;
	float realProgress;
	FString DefaultStr;

	int32 MAXDOWNLOADFILE;
	FString showStr;

	UPROPERTY()
		float progress;
public:

	UFUNCTION(BlueprintCallable, Category = "DownloadProgress")
		void SetDefaultStr(const FString& str);
	void CallbackNotifyJsonProgress();
	void CallbackSystemJsonsFinished();

	UFUNCTION(BlueprintCallable, Category = "DownloadProgress")
		void CallbackDownloadOtherProgress(FString inFileName, float inPercent);
	UFUNCTION(BlueprintCallable, Category = "DownloadProgress")
		void LocalAssetCheckFinished();
	void SetProgress(float progress);

	float GetProgress();
	void UpdateShowProgress(float progress);
	void UpdateShowText();

	void SetShowText(const FString& str);
	void ChangeProgress();

	UFUNCTION(BlueprintCallable, Category = "DownloadProgress")
		void StartDownload();
	void StartDownloadTwo();
	void DownloadFinished();
	void AddStep();
	void AddPart();
	void Reset();

	UFUNCTION(BlueprintCallable, Category = "DownloadProgress")
		float 	GetPercentProgress() { return progress; }
	UFUNCTION(BlueprintCallable, Category = "DownloadProgress")
		 void	SetPercentProgress(float _Percent) {  progress = _Percent; }

	UPROPERTY(BlueprintAssignable)
		FOnUpdateProgress onUpdateProgress;
	UPROPERTY(BlueprintAssignable)
		FOnUpdateShowText onUpdateShowText;
	UPROPERTY(BlueprintAssignable)
		FOnDownloadFinishedEvent onDownloadFinished;
	
};
