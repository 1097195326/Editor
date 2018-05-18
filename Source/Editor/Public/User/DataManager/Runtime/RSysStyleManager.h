// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRStyleManager.h"
/**
 * 
 */
class EDITOR_API RSysStyleManager :public BaseRStyleManager
{
private:

	// 单利. 
	static RSysStyleManager *   m_pInstance;

public:

	static RSysStyleManager * GetInstance();

public:

	RSysStyleManager();

	virtual ~RSysStyleManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

public:

	// 开始加载推荐款数据.
	void LoadStyle(int32 _pageIndex, int32 _pageSize, TArray<int32> _catIds = {});

	void LoadStyleById(int32 _id);
private:

	void OnLoadStyleByIdFinish(msg_ptr _msg);
	void OnLoadStyleFinish(msg_ptr _msg);
};
