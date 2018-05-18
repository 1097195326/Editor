// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "DesignModule.h"
#include "Win_StyleSpecialButton.h"
#include "Win_StyleSpecialSign.h"
#include "Win_StyleSpecialShow.generated.h"


/**
 * 
 */

UCLASS()
class EDITOR_API UWin_StyleSpecialShow : public UBaseUI
{
	GENERATED_BODY()
	
		UWin_StyleSpecialShow(const FObjectInitializer& ObjectInitializer);
private:
	TArray<UWin_StyleSpecialButton*>  m_AllShowButtons;
	TArray<UWin_StyleSpecialSign*>		m_AllShowSigns;

	UScrollBox *	m_ScrollBox;
	UCanvasPanel *	m_SignPanel;

	FTimerHandle  m_TimerHandle;
	FTimerHandle  m_SignTimerHandle;

	class UWin_RecommendStyleUMG * m_Parent;

	int32		m_SizeNum;
public:

	void SetLogicParent(class UWin_RecommendStyleUMG * parent);

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;

	void TickByTimter();

	UFUNCTION()
	void OnUserScrolledEvent(float _OffSet);
};
