// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "Win_DownloadProgressPage.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API UWin_DownloadProgressPage : public UBaseUI
{
	GENERATED_BODY()
	
public:
	UWin_DownloadProgressPage();
	
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();


	virtual void OnSetParams(int32 _pid, FString & _content, ...);


	virtual void OnTick(float InDeltaTimeSecond);

	void StartDownload();
	void UpdateProgress(float _Progress);
	void UpdateShowText(FString _ShowText);
protected:

	//UWin_DownloadProgressBar* m_ProgressBar;
	UTextBlock*          m_progresstip;
	UTextBlock*          m_tip;
};
