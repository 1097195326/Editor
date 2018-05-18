// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseRuntimeManager.h"
/**
 * 
 */
class EDITOR_API RStyleComponentManager :public GBaseRuntimeManager
{
private:
	// 单利. 
	static RStyleComponentManager *   m_pInstance;

public:
	static RStyleComponentManager * GetInstance();

public:
	
	RStyleComponentManager();

	virtual ~RStyleComponentManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;
};
