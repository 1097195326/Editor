// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WebBrowser.h"
#include "System/UI/BaseUI.h"
#include "Win_WebPage.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API UWin_WebPage : public UBaseUI
{
	GENERATED_BODY()
	
public:
	UWin_WebPage(const FObjectInitializer& ObjectInitializer);

	//	¸´Ð´BaseUI.
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);

	virtual void OnBackstageUI();
	
	void OnCloseWeb(int32 _pid);
public:
	UButton * m_BtnClose;

	UImage * m_ImageWeb;		//	¶þÎ¬ÂëÍ¼Æ¬.

	UWebBrowser * m_WebBuy;
};
