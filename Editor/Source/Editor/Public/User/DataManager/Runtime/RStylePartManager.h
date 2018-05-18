// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseRuntimeManager.h"
/**
 * 
 */
class EDITOR_API RStylePartManager :public GBaseRuntimeManager
{
private:
	// 单利. 
	static RStylePartManager *   m_pInstance;

public:
	static RStylePartManager * GetInstance();

public:

	RStylePartManager();

	virtual ~RStylePartManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;

};
