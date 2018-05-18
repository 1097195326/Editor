// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseTemplateManager.h"
/**
 * 
 */
class EDITOR_API TSuitAccessoriesManager:public GBaseTemplateManager
{
private:
	// 单利. 
	static TSuitAccessoriesManager *   m_pInstance;

public:
	static TSuitAccessoriesManager * GetInstance();

public:
	
	TSuitAccessoriesManager();

	virtual ~TSuitAccessoriesManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;
};
