// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuitFabricColorT.h"
#include "GBaseTemplateManager.h"
/**
 * 
 */
class EDITOR_API TSuitFabricManager :public GBaseTemplateManager
{
private:
	// 单利. 
	static TSuitFabricManager *   m_pInstance;

public:
	static TSuitFabricManager * GetInstance();

public:
	
	TSuitFabricManager();

	virtual ~TSuitFabricManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;

	// 返回可以继承花色.
	int32 CheckExternColor(int32 _colorTid, int32 _fabricTid);

	// 获取面料的默认推荐花色.
	SuitFabricColorT * GetDefaultFabricColor(int32 _fabricTid);
};
