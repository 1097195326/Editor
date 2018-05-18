// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoginModule.h"
#include "System/UI/BaseUI.h"
#include "LanguageSetting.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API ULanguageSetting : public UBaseUI
{
	GENERATED_BODY()

private:
	//	LoginModule.
	LoginModule * m_LoginModule;

public:
	ULanguageSetting(const FObjectInitializer& ObjectInitializer);

	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);

	//	语言设置按钮--点击事件.
	void SelectLanguage(int32 _pid);

	//	获取当前语言.
	//void GetCurrentQuality();

protected:

	UButton* m_BtnChinese;	//	中文.
	UButton* m_BtnEnglish;	//	英文.

	UTextBlock* m_TextLanguage;	//	语言设置字段.
	UTextBlock* m_TextChinese;	//	中文.
	UTextBlock* m_TextEnglish;	//	英文.
	
	
	
};
