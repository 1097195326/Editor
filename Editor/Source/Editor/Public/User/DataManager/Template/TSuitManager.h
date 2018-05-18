// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuitT.h"
#include "GBaseTemplateManager.h"
/**
 * 
 */
class EDITOR_API TSuitManager :public GBaseTemplateManager
{
private:
	// 单利. 
	static TSuitManager *   m_pInstance;

public:
	static TSuitManager * GetInstance();

public:
	
	TSuitManager();

	virtual ~TSuitManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;


};
