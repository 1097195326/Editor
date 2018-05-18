// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "DesignModule.h"
#include "StyleSpecialButton.h"
#include "StyleSpecialSign.h"
#include "StyleSpecialShow.generated.h"


/**
 * 
 */

UCLASS()
class EDITOR_API UStyleSpecialShow : public UBaseUI
{
	GENERATED_BODY()
	
		UStyleSpecialShow(const FObjectInitializer& ObjectInitializer);
private:
	TArray<UStyleSpecialButton*>  m_AllShowButtons;
	TArray<UStyleSpecialSign*>		m_AllShowSigns;

	UScrollBox *	m_ScrollBox;
	UCanvasPanel *	m_SignPanel;

	FTimerHandle  m_TimerHandle;
	FTimerHandle  m_SignTimerHandle;

	class URecommendStyleUMG * m_Parent;

	int32		m_SizeNum;
public:

	void SetLogicParent(class URecommendStyleUMG * parent);

	virtual bool OnStart() override;

	virtual void OnInit() override;

	virtual void OnRelease() override;

	virtual void OnSetParams(int32 _pid, FString & _content, ...) override;

	virtual void OnTick(float InDeltaTimeSecond) override;

	void TickByTimter();

	UFUNCTION()
	void OnUserScrolledEvent(float _OffSet);
};
