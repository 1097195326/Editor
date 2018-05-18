// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginModule.h"
#include "System/UI/BaseUI.h"
#include "DisplaySetting.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API UDisplaySetting : public UBaseUI
{
	GENERATED_BODY()

private:

	//	LoginModule.
	LoginModule * m_LoginModule;

public:
	UDisplaySetting(const FObjectInitializer& ObjectInitializer);

	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);

	//	画质设置按钮--点击事件.
	void SelectQualityLevel(int32 _pid);

	//	画质设置事件.
	void SelectClick(EQualityLevel _level);

	//	获取当前画质.
	EQualityLevel GetCurrentQuality();

protected:

	UButton* m_BtnQualityLow;	//	画质低.
	UButton* m_BtnQualityHigh;	//	画质中.
	UButton* m_BtnQualityHigher;	//	画质高.

	UTextBlock* m_TextQualityLevel;	//	画质设置字段.
	UTextBlock* m_TextQualityLow;	//	画质低字段.
	UTextBlock* m_TextQualityHigh;	//	画质中字段.
	UTextBlock* m_TextQualityHigher;	//	画质高字段.

	UImage* m_ImageQualityLow;		//	画质低选中.
	UImage* m_ImageQualityHigh;		//	画质中选中.
	UImage* m_ImageQualityHigher;	//	画质高选中.
	
};
