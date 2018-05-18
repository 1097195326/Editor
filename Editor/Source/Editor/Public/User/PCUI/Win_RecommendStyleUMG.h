// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainModule.h"
#include "Win_SelectCell.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "Win_ButtonSuitType.h"
#include "Win_RecommendStyleUMG.generated.h"


/**
 * 
 */
UCLASS()
class UWin_RecommendStyleUMG : public UBaseUI
{
	GENERATED_BODY()

private:
	TArray<UWin_SelectCell *> m_AllSyles;
	
	ResHttp		m_ResHttp;
	int32		m_CurrentCatId;
	
public:

	//	¸´Ð´BaseUI.
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);
	
	void OnClick(int _id);

	void UpdateHead();

	void UpdateShow();

	UWin_ButtonSuitType* CreateNewHead(FString Name,bool Last,int CategooryID);

	void ShowTypeStyle(int32 id);

	bool CheckCanLoad();

	UPROPERTY(BlueprintReadOnly)
		bool mIsLoading;

	UFUNCTION(BlueprintCallable,Category = AddPage)
		void AddTypeStyle();
	
	void CreateSelectCell();

	void BackToMainPage(int32 id);

	void EnterRecommend();

	void OnRecommendSuitIconFinished(msg_ptr _msg);

public:
	static	int32	m_ParentId;
	UScrollBox*		   m_Head;
	UGridPanel*        m_GridPanelSuit;
	UGridPanel*        m_HeadGrid;
	UScrollBox*        m_ScrollBoxItem;
	UGridPanel*		   m_GridPanelSuitItem;
	UScrollBox*        m_ScrollBoxItem1;
	UGridPanel*		   m_GridPanelSuitItem1;
	UScrollBox*        m_ScrollBoxItem2;
	UGridPanel*		   m_GridPanelSuitItem2;
	UCanvasPanel*	   m_SpecialShowView;
	UButton *		   m_BackButton;

	TArray<UWin_ButtonSuitType*>   ArrButtonSuitType;
	TArray<UDownloadImageTool *> m_DownloadImageTools;
};
