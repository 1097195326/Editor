// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "ButtonSuitType.generated.h"

/**
 * 
 */
UCLASS()
class EDITOR_API UButtonSuitType : public UBaseUI
{
	GENERATED_BODY()
public: // 重写以下的类.
	UButtonSuitType(const FObjectInitializer& ObjectInitializer);
	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	// 被传入参数的列表.
	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;
	void ClientButtonEvent(int32 _pid);//按钮点击事件.
	// update.
	virtual void OnTick(float InDeltaTimeSecond) override;
	

	void SetNormal();
	void SetData(FString _name, int32 _categoryID);
	void SetHover();
	void SetSelect();
	int32 GetID() { return m_CategoryID; };
public:

	UCanvasPanel*   m_CanvasPanelSuit;

	UTextBlock*     m_TextBlockSuit;

	UButton*        m_ButtonSuit;
	UImage*        m_ImageSuit;
	bool          m_IsSelect;
	int32       m_CategoryID;
	FString     m_Name;
};
