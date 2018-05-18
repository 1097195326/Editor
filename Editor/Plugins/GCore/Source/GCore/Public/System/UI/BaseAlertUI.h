// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "UIAlertDelegate.h"
#include "BaseAlertUI.generated.h"

/**
 * 
 */
UCLASS()
class GCORE_API UBaseAlertUI : public UBaseUI
{
	GENERATED_BODY()

protected:
	UButton* m_BtnCancel = nullptr;

	UButton* m_BtnConfirm = nullptr;

	UButton* m_ButtonClose = nullptr;

	UTextBlock* m_TextContent = nullptr;

	UIAlertDelegate * m_alertDelegate = nullptr;

	// 倒计时,=0表示不关，>0就到时间自动关闭UI.
	float   m_second = 0;

	// timer编号，用于关闭timer用.
	int32 m_updateID = 0;

	// 显示的内容.
	FString m_content;

public:

	// 释放UI事件.
	virtual void NativeDestruct() override; 

protected:
	// 确认按钮点击事件.
	virtual void ClickBtnConfirm();

	virtual void ClickBtnCancel();

	virtual void ClickBtnClose();

	virtual void OnInitAlertUI();

public:
	// 初始化.
	virtual void InitAlertUI(float _second, const FString & _content, UIAlertDelegate * _alertDelegate);

	
public:
	UBaseAlertUI(const FObjectInitializer& ObjectInitializer);
	
private:
	bool OnFinishTask(float _delateTime);

	void StopTask();
};
