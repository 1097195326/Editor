// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseRuntimeManager.h"
/**
 * 
 */
class EDITOR_API RStyleAccessoriesManager:public GBaseRuntimeManager
{

private:
	// 单利. 
	static RStyleAccessoriesManager *   m_pInstance;

public:
	static RStyleAccessoriesManager * GetInstance();

public:
	
	RStyleAccessoriesManager();
	
	virtual ~RStyleAccessoriesManager();

public:
	
	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;
};
