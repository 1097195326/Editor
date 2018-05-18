// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseTemplateManager.h"
/**
 * 
 */
class EDITOR_API TSuitPartManager :public GBaseTemplateManager
{
private:
	// 单利. 
	static TSuitPartManager *   m_pInstance;

public:
	static TSuitPartManager * GetInstance();

public:
	TSuitPartManager();
	virtual ~TSuitPartManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;
};
