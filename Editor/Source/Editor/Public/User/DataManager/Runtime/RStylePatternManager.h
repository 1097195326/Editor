// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GBaseRuntimeManager.h"
/**
 * 
 */
class EDITOR_API RStylePatternManager :public GBaseRuntimeManager
{
private:
	// 单利. 
	static RStylePatternManager *   m_pInstance;

public:
	static RStylePatternManager * GetInstance();

public:
	RStylePatternManager();
	
	virtual ~RStylePatternManager();

public:

	virtual void OnLoad() override;

	virtual void OnRelease() override;

	virtual void EncodeData(TSharedPtr<FJsonObject> & _content) override;
};
