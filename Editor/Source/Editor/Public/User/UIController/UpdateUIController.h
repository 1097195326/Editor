// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIController.h"
#include "HotUpdateUMG.h"
#include "LoadDataModule.h"


struct DownlaodInfoStruct
{
	//大小Mb.
	float m_Size;
	//速度Kb.
	float m_Speed;
	//进度0-1.
	float m_Progress;
	//下载内容信息.
	FString m_DownloadMsg;

};

class EDITOR_API UpdateUIController:public UIController
{
public:
	UpdateUIController();

	virtual ~UpdateUIController();

protected: // 以下类必须重写.

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	// rootview进入后台=隐藏UI.
	virtual void OnBackstageUI() override;

	// rootView 进入激活状态.
	virtual void OnActivationUI() override;


	void OnDownloadPakProgress(msg_ptr _msg);
	void OnDownloadDefaultDataProgress(msg_ptr _msg);
	void OnDownloadSuitDetailProgress(msg_ptr _msg);

	
public:
	void ShowProgressUI();
	void DownloadDefaultData();
	void DownloadSuitDetail(int32 _suitTid);
	void DownloadPak();
private:
	UBaseUI *m_HotUpdateUMG;
};
