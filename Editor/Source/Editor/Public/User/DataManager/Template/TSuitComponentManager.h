// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseTemplateManager.h"
/**
 * 
 */
class EDITOR_API TSuitComponentManager :public GBaseTemplateManager
{
private:
	// 单利. 
	static TSuitComponentManager *   m_pInstance;

public:
	static TSuitComponentManager * GetInstance();

public:

	TSuitComponentManager();
	
	virtual ~TSuitComponentManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;

};
