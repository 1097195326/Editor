// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRStyleManager.h"
/**
 * 
 */
class EDITOR_API RUserStyleManager :public BaseRStyleManager
{
private:

	// 单利. 
	static RUserStyleManager *   m_pInstance;

public:

	static RUserStyleManager * GetInstance();

public:
	RUserStyleManager();

	virtual ~RUserStyleManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:

	// 开始用户款数据.
	void LoadStyle(int32 _pageIndex, int32 _pageSize);

	void LoadStyleForMainUI(int32 _pageIndex, int32 _pageSize);

private:
	//
	void OnLoadStyleFinish(msg_ptr _msg);

	// 主界面请求的款数据回调.
	void OnLoadStyleFinishForMainUI(msg_ptr _msg);
};
