// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAlertUI.h"
#include "Popup_Window.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API UPopup_Window : public UBaseAlertUI
{
	GENERATED_BODY()

public:
	UPopup_Window(const FObjectInitializer& ObjectInitializer);

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;

public:

	// 初始化UI.
	virtual void OnInitAlertUI() override;

	//	关闭提示框.
	void OnButtonCancel(int32 _pid);  

	void OnButtonConfirm(int32 _pid);

	void OnButtonClose(int32 _pid);

};
