// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/UI/BaseUI.h"
#include "DownloadImageTool.h"
#include "MainModule.h"
#include "ButtonSuitType.h"
#include "SelectCell.h"
#include "RecommendStyleUMG.generated.h"

/**
 * 
 */
UCLASS()
class URecommendStyleUMG : public UBaseUI
{
	GENERATED_BODY()

private:
	TArray<USelectCell *> m_AllSyles;
	
	ResHttp		m_ResHttp;
	int32		m_CurrentCatId;

	bool CheckCanLoad();
	
public:
	static	int32	m_ParentId;

	UPROPERTY(BlueprintReadOnly)
		bool mIsLoading;
	//	¸´Ð´BaseUI.
	virtual bool OnStart() override;

	virtual void OnInit();

	virtual void OnRelease();

	virtual void OnSetParams(int32 _pid, FString & _content, ...);

	virtual void OnTick(float InDeltaTimeSecond);
	
	void OnClick(int _id);

	void UpdateHead();
	void UpdateShow();

	UButtonSuitType* CreateNewHead(FString Name,bool Last,int CategooryID);

	void ShowTypeStyle(int32 id);

	UFUNCTION(BlueprintCallable,Category = AddPage)
	void AddTypeStyle();
	
	void CreateSelectCell();
	void BackToMainPage(int32 id);

	void EnterRecommend();

	void OnRecommendSuitIconFinished(msg_ptr _msg);
//
public:
	UScrollBox*    m_Head;

	UGridPanel*        m_GridPanelSuit;
	UGridPanel*        m_HeadGrid;

	UPROPERTY(BlueprintReadOnly)
	UScrollBox*        m_ScrollBoxItem;

	UGridPanel*		   m_GridPanelSuitItem;

	UScrollBox*        m_ScrollBoxItem1;
	UGridPanel*		   m_GridPanelSuitItem1;
	UScrollBox*        m_ScrollBoxItem2;
	UGridPanel*		   m_GridPanelSuitItem2;

	UCanvasPanel*		m_SpecialShowView;

	UButton *		m_BackButton;

	TArray<UButtonSuitType*>   ArrButtonSuitType;
	TArray<UDownloadImageTool *> m_DownloadImageTools;
};
