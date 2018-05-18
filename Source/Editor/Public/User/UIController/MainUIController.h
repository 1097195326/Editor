 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MsgChannel.h"
#include "MainModule.h"
#include "UIController.h"
#include "RecommendStyleUMG.h"

enum class PAGE_TYPE :uint8
{
	MAIN_PAGE = 0,
	Recommend_PAGE = 1,
	MyWork_PAGE = 2,
	PersonInfo_PAGE = 3,
	Photo_PAGE = 4,
	DisplaySetting_PAGE = 5,
	LanguageSetting_PAGE = 6,
	StyleSpecialShowList = 7,
	Sport_Page = 8,
};

/**
 * 
 */
class EDITOR_API MainUIController:public UIController
{
public:
	MainUIController();

	virtual~MainUIController();

	void ChangePageTo(PAGE_TYPE _type);

protected: // 以下类必须重写.

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	// rootview进入后台=隐藏UI.
	virtual void OnBackstageUI() override;

	// rootView 进入激活状态.
	virtual void OnActivationUI() override;

private:
	UBaseUI * m_CurrentView;

	UBaseUI * m_PreView;

	UBaseUI * m_RecommentStyle;

	UBaseUI * m_MyWorks;

	UBaseUI * m_PersonInfo;

	UBaseUI * m_PhotoPage;

	UBaseUI * m_DisplaySetting;

	UBaseUI * m_LanguageSetting;
};
